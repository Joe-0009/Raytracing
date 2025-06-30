#include "../includes/minirt_app.h"
#include "../includes/parser.h"
#include <stdio.h>

void	init_parser_and_scene(t_parser *parser, t_scene *scene)
{
	parser->line = NULL;
	parser->tokens = NULL;
	parser->line_count = 0;
	parser->has_camera = FALSE;
	scene->num_objects = 0;
	scene->camera.fov = 0.0;
	scene->has_ambient = FALSE;
	scene->has_light = FALSE;
	scene->ambient.ratio = 0.0;
	scene->ambient.color.x = 0.0;
	scene->ambient.color.y = 0.0;
	scene->ambient.color.z = 0.0;
	scene->light.position.x = 0.0;
	scene->light.position.y = 0.0;
	scene->light.position.z = 0.0;
	scene->light.brightness = 0.0;
	scene->light.color.x = 0.0;
	scene->light.color.y = 0.0;
	scene->light.color.z = 0.0;
}

int	validate_extension_and_permission(const char *filename, t_scene *scene)
{
	const char	*extension;
	int			fd;

	extension = strrchr(filename, '.');
	if (!extension || ft_strncmp(extension, ".rt", 3) != 0)
	{
		printf(ERR_FILE_EXTENSION);
		free(scene);
		return (-1);
	}
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		printf(ERR_FILE_ACCESS, filename);
		free(scene);
		return (-1);
	}
	return (fd);
}

int	is_empty_line(const char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n'
			&& line[i] != '\r')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	dispatch_parse_token(char **tokens, t_scene *scene)
{
	size_t	token_len;

	token_len = strlen(tokens[0]);
	if (token_len == 1)
	{
		if (tokens[0][0] == 'A')
			return (parse_ambient(tokens, scene));
		else if (tokens[0][0] == 'C')
			return (parse_camera(tokens, scene));
		else if (tokens[0][0] == 'L')
			return (parse_light(tokens, scene));
	}
	else if (token_len == 2)
	{
		if (tokens[0][0] == 's' && tokens[0][1] == 'p')
			return (parse_sphere(tokens, scene));
		else if (tokens[0][0] == 'p' && tokens[0][1] == 'l')
			return (parse_plane(tokens, scene));
		else if (tokens[0][0] == 'c' && tokens[0][1] == 'y')
			return (parse_cylinder(tokens, scene));
		else if (tokens[0][0] == 'c' && tokens[0][1] == 'n')
			return (parse_cone(tokens, scene));
	}
	return (FALSE);
}

int	process_scene_line(t_parser *parser, t_scene *scene, char *line)
{
	int	parse_result;

	parser->line_count++;
	parser->line = line;
	if (is_empty_line(line))
	{
		free(line);
		parser->line = NULL;
		return (1);
	}
	parser->tokens = ft_split(line, " \t\n\r");
	if (!parser->tokens || !parser->tokens[0] || parser->tokens[0][0] == '#')
	{
		free(line);
		free_tokens(parser->tokens);
		parser->tokens = NULL;
		parser->line = NULL;
		return (1);
	}
	parse_result = dispatch_parse_token(parser->tokens, scene);
	if (!parse_result)
	{
		if (parser->tokens && parser->tokens[0])
		{
			printf("Error: Unknown identifier: ");
			printf("%s", parser->tokens[0]);
			printf("\n");
		}
		else
			printf("Error: Invalid line format\n");
	}
	free(line);
	free_tokens(parser->tokens);
	parser->tokens = NULL;
	parser->line = NULL;
	return (parse_result);
}

t_scene	*parse_scene_file(char *filename)
{
	t_scene		*scene;
	t_parser	parser;
	int			fd;
	char		*line;
	int			result;

	scene = (t_scene *)malloc(sizeof(t_scene));
	if (!scene)
		return (NULL);
	init_parser_and_scene(&parser, scene);
	fd = validate_extension_and_permission(filename, scene);
	if (fd == -1)
		return (NULL);
	line = get_next_line(fd);
	while (line != NULL)
	{
		result = process_scene_line(&parser, scene, line);
		if (result == 0)
			return (close(fd), free(scene), NULL);
		line = get_next_line(fd);
	}
	close(fd);
	if (parser.line_count == 0)
		return (printf("Error: Empty file\n"), free(scene), NULL);
	if (!validate_scene(scene))
		return (free(scene), NULL);
	return (scene);
}
