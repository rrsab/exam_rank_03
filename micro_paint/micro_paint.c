#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

typedef struct s_zone
{
	int width;
	int height;
	char background;
}               t_zone;

typedef struct s_list
{
	char type;
	float x;
	float y;
	float width;
	float height;
	char color;
}               t_list;

int ft_strlen(char *s)
{
	int i;

	i = 0;
	if (!s)
		return (i);
	while (s[i] != '\0')
		++i;
	return (i);
}

int fail(char *s)
{
	write(1, s, ft_strlen(s));
	return(1);
}

int free_all(FILE *file, char *s)
{
	if (s)
		free(s);
	fclose(file);
	return(1);
}

char *get_zone(FILE *file, t_zone *zone)
{
	int i;
	char *array;

	if ((i = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background)) != 3)
		return (NULL);
	if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
		return (NULL);
	if (!(array = (char *)malloc(sizeof(char) * (zone->width * zone->height))))
		return (NULL);
	i = -1;
	while (++i < zone->height * zone->width)
		array[i] = zone->background;
	return (array);
}

int is_rec(float x, float y, t_list *tmp)
{
	float check = 1.00000000;

	if ((x < tmp->x) || ((tmp->x + tmp->width) < x) || (y < tmp->y) || ((tmp->y + tmp->height) < y))
		return (0);
	if (((x - tmp->x) < check) || (((tmp->x + tmp->width) - x) < check) || ((y - tmp->y) < check) || (((tmp->y + tmp->height) - y) < check))
		return (2);
	return (1);
}

void get_draw(t_zone *zone, t_list *tmp, char *array)
{
	int x, y, rec;

	y = 0;
	while (y < zone->height)
	{
		x = 0;
		while (x < zone->width)
		{
			rec = is_rec((float)x, (float)y, tmp);
			if ((rec == 2 && tmp->type == 'r') || (rec && tmp->type == 'R'))
				array[(y * zone->width) + x] = tmp->color;
			++x;
		}
		++y;
	}
}

int drawing(FILE *file, t_zone *zone, char *array)
{
	int count;
	t_list tmp;

	while ((count = fscanf(file, "%c %f %f %f %f %c\n", &tmp.type, &tmp.x, &tmp.y, &tmp.width, &tmp.height, &tmp.color)) == 6)
	{
		if (tmp.width <= 0.00000000 || tmp.height <= 0.00000000)
			return (0);
		if (tmp.type != 'r' && tmp.type != 'R')
			return (0);
		get_draw(zone, &tmp, array);
	}
	if (count != -1)
		return (0);
	return (1);
}

void  print_array(t_zone *zone, char *array)
{
	int i;

	i = 0;
	while (i < zone->height)
	{
		write(1, array + (i * zone->width), zone->width);
		write(1, "\n", 1);
		++i;
	}
}

int main(int argc, char **argv)
{
	FILE *file;
	t_zone zone;
	char *array;

	if (argc != 2)
		return (fail("Error: argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return (fail("Error: Operation file corrupted\n"));
	if (!(array = get_zone(file, &zone)))
		return (free_all(file, NULL) && fail("Error: Operation file corrupted\n"));
	if (!(drawing(file, &zone, array)))
		return (free_all(file, array) && fail("Error: Operation file corrupted\n"));
	print_array(&zone, array);
	free_all(file, array);
	return (0);
}

