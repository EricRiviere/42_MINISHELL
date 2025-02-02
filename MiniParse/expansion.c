#include "minishell.h"

char *ft_strjoin_free(char *s1, char *s2)
{
    char *result;

    if (!s1 && s2)
        return (ft_strdup(s2));
    if (!s2)
        return (s1);
    result = ft_strjoin(s1, s2);
    free(s1);
    return (result);
}

int is_valid_env_char(char c)
{
    return (ft_isalpha(c) || ft_isdigit(c) || c == '_');
}

char *expand_value(t_token *token, t_env *env_lst)
{
    int     i;
    int     start;
    char    *temp;
    char    *sub_expand;
    char    *expand;

    if (token->prev && ft_strncmp(token->prev->value, "<<", 2) == 0)
        return (ft_strdup(token->value));
    i = 0;
    expand = NULL;
    while (token->value[i])
    {
        // Caso: texto literal antes de un '$'
        if (token->value[i] != '$')
        {
            start = i;
            while (token->value[i] && token->value[i] != '$')
                i++;
            temp = ft_strndup(&token->value[start], i - start); // Extrae texto literal
            expand = ft_strjoin_free(expand, temp);   // Une y libera `expand`
            free(temp);
        }
        // Caso: expansión de variable
        else if (token->value[i++] == '$')
        {
            if (token->is_quote == 32 && (!token->value[i] || token->value[i] == ' ' || !is_valid_env_char(token->value[i]))) // Caso literal
            {
                temp = ft_strdup("$"); // Conserva el símbolo literal
                expand = ft_strjoin_free(expand, temp);
                free(temp);
            }
            else if (token->is_quote == 0 && (!token->value[i] || !is_valid_env_char(token->value[i]))) // Caso literal
            {
                temp = ft_strdup("$"); // Conserva el símbolo literal
                expand = ft_strjoin_free(expand, temp);
                free(temp);
            }
            else
            {
                start = i;
                while (token->value[i] && is_valid_env_char(token->value[i]))
                    i++;
                temp = ft_strndup(&token->value[start], i - start); // Nombre de la variable
                sub_expand = get_env_value(temp, env_lst); // Busca en la lista de variables
                free(temp);
                if (token->type == WORD && !sub_expand)                             // Libera `temp`
                    sub_expand = ft_strdup("");
                expand = ft_strjoin_free(expand, sub_expand); // Une y libera `expand`
            }
        }
    }
    return (expand);
}

void expand_variables(t_token *token, t_env *env_lst)
{
    char *new_value;

    if (token->expand)
    {
        if (token->type == QUOTED || token->type == WORD)
            new_value = expand_value(token, env_lst);
        else
            return;
        // Libera el valor anterior y actualiza con el nuevo
        free(token->value);
        token->value = new_value;
    }
}