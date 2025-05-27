#include "../includes/minirt.h"
#include "../includes/parser.h"
#include <stdio.h>

/**
 * Initializes both a t_parser and a t_scene structure.
 * @param parser Pointer to the t_parser structure to initialize.
 * @param scene Pointer to the t_scene structure to initialize.
 */
void	init_parser_and_scene(t_parser *parser, t_scene *scene)
{
	parser->line = NULL;
	parser->tokens = NULL;
	parser->line_count = 0;
	parser->has_camera = FALSE;
	scene->num_objects = 0;
	scene->camera.fov = 0.0;
}

/**
 * Checks the file extension and read permission for the given filename.
 * Prints an error and frees the scene if invalid.
 * @param filename The path to the file to check.
 * @param scene Pointer to the scene to free on error.
 * @return File descriptor if valid, -1 on error.
 */
int	validate_extension_and_permission(const char *filename, t_scene *scene)
{
	const char	*extension;
	int			fd;

	extension = strrchr(filename, '.');
	if (!extension || ft_strncmp(extension, ".rt", 3) != 0)
	{
		ft_fprintf_fd(2, ERR_FILE_EXTENSION);
		free(scene);
		return (-1);
	}
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_fprintf_fd(2, ERR_FILE_ACCESS, filename);
		free(scene);
		return (-1);
	}
	return (fd);
}

/**
 * Checks if a line is empty or contains only whitespace characters.
 * @param line The line to check.
 * @return TRUE if the line is empty/whitespace-only, FALSE otherwise.
 */
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

/**
 * Dispatches the parsing of a token to the appropriate element parser.
 * @param tokens The tokenized line (tokens[0] is the identifier).
 * @param scene Pointer to the scene structure to populate.
 * @return TRUE if parsing was successful, FALSE otherwise.
 */
int	dispatch_parse_token(char **tokens, t_scene *scene)
{
	size_t	token_len;

	token_len = strlen(tokens[0]);
	if (token_len == 1)
	{
		if (tokens[0][0] == 'C')
			return (parse_camera(tokens, scene));
	}
	else if (token_len == 2)
	{
		if (tokens[0][0] == 's' && tokens[0][1] == 'p')
			return (parse_sphere(tokens, scene));
		else if (tokens[0][0] == 'p' && tokens[0][1] == 'l')
			return (parse_plane(tokens, scene));
		else if (tokens[0][0] == 'c' && tokens[0][1] == 'y')
			return (parse_cylinder(tokens, scene));
	}
	return (FALSE);
}

/**
 * Processes a single line from the scene file, handling tokenization, comments,
	and element parsing.
 * @param parser Pointer to the parser structure.
 * @param scene Pointer to the scene structure.
 * @param line The line to process (will be freed).
 * @return 1 to continue, 0 on error (caller should handle cleanup).
 */
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
			ft_fprintf_fd(2, "Error: Unknown identifier: ");
			ft_fprintf_fd(2, parser->tokens[0]);
			ft_fprintf_fd(2, "\n");
		}
		else
			ft_fprintf_fd(2, "Error: Invalid line format\n");
	}
	free(line);
	free_tokens(parser->tokens);
	parser->tokens = NULL;
	parser->line = NULL;
	return (parse_result);
}

/**
 * Parses the scene file and returns a pointer to the scene structure.
 * Handles file validation, parsing, and error reporting.
 * @param filename The path to the scene file.
 * @return Pointer to the parsed scene, or NULL on error.
 */
t_scene	*parse_scene_file(char *filename)
{
	t_scene		*scene;
	t_parser	parser;
	int			fd;
	char		*line;
	int			result;

	scene = (t_scene *)malloc(sizeof(t_scene));
	if (!scene)
	{
		return (NULL);
	}
	
	init_parser_and_scene(&parser, scene);
	
	fd = validate_extension_and_permission(filename, scene);
	if (fd == -1)
	{
		return (NULL);
	}
	
	line = get_next_line(fd);
	while (line != NULL)
	{
		result = process_scene_line(&parser, scene, line);
		if (result == 0)
		{
			return (close(fd), free(scene), NULL);
		}
		line = get_next_line(fd);
	}
	close(fd);
	
	if (parser.line_count == 0)
	{
		return (ft_fprintf_fd(2, "Error: Empty file\n"), free(scene), NULL);
	}
	
	if (!validate_scene(scene))
	{
		return (free(scene), NULL);
	}
	
	return (scene);
}
