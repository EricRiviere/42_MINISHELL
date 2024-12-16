#include "minishell.h"

// abre fds infinitos pero no se cierra cuando devuelve su nombre
// faltaria un strncmp que chequee si line == heredoc name y se cierre
// hacer un dup para redirigir el output en caso de que se rediriga el heredoc a outfile
static void process_heredoc(t_token *heredoc_token)
{
    int fd;
    char *line;
    fd = open("/tmp/heredoc_pipe", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
    {
        ft_putchar_fd("Error opening heredoc", 2);
        return ;
    }
    while (1)
    {
        line = readline(">");
        if (line == NULL)
            break ;
        write(fd, line, ft_strlen(line));
        write(fd, "\n" ,1);
        free(line);
    }
    close(fd);
}
int main(int argc, char **argv, char **env)
{
    char    *line;
    t_token *tkn_lst;
    t_token *curr_tkn;
    t_env   *env_lst;

    (void)argv;
    if (argc != 1)
    {
        printf("Wrong number of arguments\n");
        return (1);
    }
    env_lst = init_env_list(env);
    while (1)
    {
        line = readline("minishell> ");
        if (!line)
            break;
        if (line)
        {
            if (ft_strncmp(line, "env", 3) == 0)
                print_env_list(env_lst);
            add_history(line);
            tkn_lst = tokenize(line);
            if (tkn_lst && !syntax_check(tkn_lst))
            {
                free_tkn_lst(tkn_lst);
                free(line);
                continue;
            }
            if (tkn_lst)
            {
                curr_tkn = tkn_lst;
                while (curr_tkn)
                {
                    if (curr_tkn->type == HEREDOC)
                    {
                        process_heredoc(curr_tkn);
                    }
                    else
                    {
                        expand_variables(curr_tkn, env_lst);
                    }
                    curr_tkn = curr_tkn->next;
                }
                curr_tkn = tkn_lst;
            }
            if (tkn_lst)
            {
                curr_tkn = tkn_lst;
                while(curr_tkn)
                {
                    printf("Token value: *%s*\n", curr_tkn->value);
                    printf("is quote? --> %d\n", curr_tkn->is_quote);
                    printf("expands? --> %d\n", curr_tkn->expand);
                    printf("Number of spaces --> %d\n", curr_tkn->has_space);
                    printf("type: %u\n", curr_tkn->type);
                    curr_tkn = curr_tkn->next;
                }
            }
            free_tkn_lst(tkn_lst);
        }
        free(line);
    }
    free_env_list(env_lst);
    return (0);
}