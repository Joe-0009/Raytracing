#include "../includes/minirt_app_bonus.h"
#include "../includes/parser_bonus.h"
#include <stdio.h>

/*
** Counts the number of tokens in a NULL-terminated array.
*/
static int	count_tokens(char **tokens)
{
	int	count;

	count = 0;
	while (tokens[count])
		count++;
	return (count);
}

/*
** Initializes the parser and scene structures to default values.
*/
void	init_parser_and_scene(t_parser *parser, t_scene *scene)
{
	parser->line = NULL;
	parser->tokens = NULL;
	parser->line_count = 0;
	parser->has_camera = FALSE;
	scene->num_objects = 0;
	scene->selected_obj = 0;
	scene->camera.fov = 0.0;
	scene->has_ambient = FALSE;
	scene->ambient.ratio = 0.0;
	scene->ambient.color.x = 0.0;
	scene->ambient.color.y = 0.0;
	scene->ambient.color.z = 0.0;
	scene->nbr_of_lights = 0;
}

/*
** Validates the file extension and checks read permissions for the scene file.
*/
int	validate_extension_and_permission(const char *filename, t_scene *scene)
{
	const char	*extension;
	int			fd;

	extension = ft_strrchr(filename, '.');
	if (!extension || ft_strncmp(extension, ".rt", 4) != 0)
	{
		printf(ERR_FILE_EXTENSION);
		ft_free_scene(&scene);
		return (-1);
	}
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		printf(ERR_FILE_ACCESS, filename);
		ft_free_scene(&scene);
		return (-1);
	}
	return (fd);
}

/*
** Dispatches the parsing of a line's tokens to the appropriate object parser based on the identifier.
*/
int	dispatch_parse_token(char **tokens, t_scene *scene, int tokens_count)
{
	size_t	token_len;

	token_len = strlen(tokens[0]);
	if (token_len == 1)
	{
		if (tokens[0][0] == 'A')
			return (parse_ambient(tokens, scene, tokens_count));
		else if (tokens[0][0] == 'C')
			return (parse_camera(tokens, scene, tokens_count));
		else if (tokens[0][0] == 'L')
			return (parse_light(tokens, scene, tokens_count));
	}
	else if (token_len == 2)
	{
		if (tokens[0][0] == 's' && tokens[0][1] == 'p')
			return (parse_sphere(tokens, scene, tokens_count));
		else if (tokens[0][0] == 'p' && tokens[0][1] == 'l')
			return (parse_plane(tokens, scene, tokens_count));
		else if (tokens[0][0] == 'c' && tokens[0][1] == 'y')
			return (parse_cylinder(tokens, scene, tokens_count));
		else if (tokens[0][0] == 'c' && tokens[0][1] == 'n')
			return (parse_cone(tokens, scene, tokens_count));
	}
	return (FALSE);
}

/*
** Processes a single line from the scene file, splitting it into tokens and parsing the object.
*/
int	process_scene_line(t_parser *parser, t_scene *scene, char *line)
{
	int	parse_result;
	int	tokens_count;

	parser->line_count++;
	parser->line = line;
	if (is_empty_line(line))
	{
		return (ft_free((void **)&line), 1);
	}
	parser->tokens = ft_split(line, " \t\n\r");
	tokens_count = count_tokens(parser->tokens);
	if (!parser->tokens || !parser->tokens[0] || parser->tokens[0][0] == '#')
	{
		return (ft_free((void **)&line), free_tokens(parser->tokens), 1);
	}
	parse_result = dispatch_parse_token(parser->tokens, scene, tokens_count);
	if (!parse_result)
	{
		if (parser->tokens && parser->tokens[0])
		{
			printf("Error: Unknown identifier: ");
			printf("%s\n", parser->tokens[0]);
		}
	}
	return (ft_free((void **)&line), free_tokens(parser->tokens), parse_result);
}

/*
** Parses the entire scene file and returns a pointer to the populated scene structure.
*/
t_scene	*parse_scene_file(char *filename)
{
	t_scene		*scene;
	t_parser	parser;
	int			fd;
	char		*line;
	int			result;

	scene = (t_scene *)malloc(sizeof(t_scene));
	init_parser_and_scene(&parser, scene);
	fd = validate_extension_and_permission(filename, scene);
	if (fd == -1)
		return (NULL);
	line = get_next_line(fd);
	while (line != NULL)
	{
		result = process_scene_line(&parser, scene, line);
		if (result == 0)
			return (close(fd), ft_free_scene(&scene), NULL);
		line = get_next_line(fd);
	}
	close(fd);
	if (parser.line_count == 0)
		return (printf("Error: Empty file\n"), ft_free_scene(&scene), NULL);
	if (!validate_scene(scene))
		return (ft_free_scene(&scene), NULL);
	return (scene);
}
