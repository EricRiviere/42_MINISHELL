#include "minishell.h"

void preprocess_tokens(t_token **tkn_lst)
{
    t_token *curr = *tkn_lst;

    while (curr)
    {
        if ((curr->type == WORD || curr->type == QUOTED) && curr->has_space == 0)
        {
            t_token *prev = curr->prev;
            if (prev && (prev->type == WORD || prev->type == QUOTED)) // Validar tipo del token anterior
            {
                char *temp = prev->value;
                prev->value = ft_strjoin(prev->value, curr->value); // Concatenar al token anterior
                free(temp);

                // Eliminar el token actual después de la concatenación
                prev->next = curr->next;
                if (curr->next)
                    curr->next->prev = prev;
                free(curr->value);
                free(curr);
                curr = prev->next;
                continue;
            }
        }
        curr = curr->next;
    }
}

t_command **commands(t_token *tkn_lst)
{
    t_token *curr_tkn = tkn_lst;
    t_command **cmd_list = malloc(sizeof(t_command *) * 100); // Máximo 100 comandos
    int cmd_index = 0;

    while (curr_tkn)
    {
        t_command *cmd = malloc(sizeof(t_command));
        t_redir *redir = malloc(sizeof(t_redir));
        char **args = malloc(sizeof(char *) * 100); // Máximo 100 argumentos
        char **operator = malloc(sizeof(char *) * 50); // Máximo 50 redirecciones
        char **file = malloc(sizeof(char *) * 50);
        int arg_index = 0, op_index = 0;

        cmd->cmd = NULL;
        cmd->args = args;
        cmd->redirections = redir;

        while (curr_tkn && ft_strncmp(curr_tkn->value, "|", 1) != 0)
        {
            if (curr_tkn->type == 2 && curr_tkn->next) // Redirección
            {
                operator[op_index] = ft_strdup(curr_tkn->value);
                file[op_index] = ft_strdup(curr_tkn->next->value);
                op_index++;
                curr_tkn = curr_tkn->next->next;
            }
            else if (curr_tkn->type == 0 || curr_tkn->type == 1) // Word o Quote
            {
                if (!cmd->cmd) // Asignar el primer token como comando
                    cmd->cmd = strdup(curr_tkn->value);
                else // Agregar como argumento
                {
                    args[arg_index] = ft_strdup(curr_tkn->value);
                    arg_index++;
                }
                curr_tkn = curr_tkn->next;
            }
            else
            {
                curr_tkn = curr_tkn->next;
            }
        }

        // Finalizar listas
        args[arg_index] = NULL;
        operator[op_index] = NULL;
        file[op_index] = NULL;
        redir->operator = operator;
        redir->file = file;

        cmd_list[cmd_index] = cmd;
        cmd_index++;

        if (curr_tkn && ft_strncmp(curr_tkn->value, "|", 1) == 0)
            curr_tkn = curr_tkn->next;
    }

    cmd_list[cmd_index] = NULL;
    return cmd_list;
}

void free_cmd_list(t_command **cmd_list)
{
    int i = 0;

    while (cmd_list[i])
    {
        int j = 0;
        while (cmd_list[i]->args[j])
        {
            free(cmd_list[i]->args[j]);
            j++;
        }
        free(cmd_list[i]->args);

        j = 0;
        while (cmd_list[i]->redirections->operator[j])
        {
            free(cmd_list[i]->redirections->operator[j]);
            j++;
        }
        free(cmd_list[i]->redirections->operator);

        j = 0;
        while (cmd_list[i]->redirections->file[j])
        {
            free(cmd_list[i]->redirections->file[j]);
            j++;
        }
        free(cmd_list[i]->redirections->file);

        free(cmd_list[i]->cmd);
        free(cmd_list[i]);
        i++;
    }
    free(cmd_list);
}

