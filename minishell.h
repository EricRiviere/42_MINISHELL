#ifndef MINISHELL_H
# define MINISHELL_H
//readline --> compilar con -lreadline
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
//------------------
# include <stdlib.h> //malloc
# include <unistd.h> // write
# include "libft/libft.h"
# include <fcntl.h>

//------------------ TOKEN TYPE ENUM
typedef enum    s_type
{
    WORD,
    QUOTED,
    OPERATOR
}   t_type;
//------------------
//------------------ OPERATOR TYPE ENUM
typedef enum    s_operator
{
    INPUT,
    OUTPUT,
    HEREDOC,
    APPEND,
    PIPE
}   t_operator;
//------------------
//------------------ TOKEN STRUCT
typedef struct  s_token t_token;

struct s_token
{
    char    *value;
    short   is_quote;
    short   expand;
    int     has_space;
    t_type  type;
    t_token *next;
    t_token *prev;
};
//------------------
//------------------ ARGUMENTS STRUCT
typedef struct  s_args
{
    char    *line;
    int     *i;
    t_token **tkn_lst;
    int     spaces;
}   t_args;
//------------------
//-------------------ENV STRUCT
typedef struct s_env
{
    char    *key;
    char    *value;
    struct s_env *next;
}   t_env;
//------------------
//------------------ LIBFT FUNCTIONS
size_t  ft_strlen(const char *str);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *str);
void	ft_putstr_fd(char *s, int fd);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
int	ft_isalpha(int c);
int	ft_isdigit(int c);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
char	*ft_strjoin(char const *s1, char const *s2);
//------------------ UTILS FUNCTIONS
char    *ft_strndup(const char *str, size_t len);
int is_space(char c);
int is_special_char(char c);
//------------------ ENVIROMENT FUNCTIONS
void    free_env_list(t_env *env_list);
void    add_env_variable(t_env **env_list, char *key, char *value);
t_env *init_env_list(char **env);
void    print_env_list(t_env *env_list);
char    *get_env_value(char *str, t_env *env_list);
//------------------ TOKENIZE FUNCTIONS
t_args  *pass_args(char *line, int *i, t_token **tkn_lst, int spaces);
t_token *init_token(t_type type);
void    add_token(t_token **tkn_lst, t_token *token);
void    free_tkn_lst(t_token *tkn_lst);
t_token    *parse_quote(t_args *args, char quote, int start, int end);
t_token   *manage_quote(t_args *args, int start);
t_token *parse_operator(t_operator operator, int spaces);
t_operator get_operator_type(const char *line, int i);
int create_operator_token(t_args *args, t_operator operator, int increment);
t_token *manage_operator(t_args *args);
t_token *manage_word(t_args *args); //*refactorizar muy larga
void skip_spaces(const char *line, int *i, int *spaces);
int process_token(t_args *args);
t_token *tokenize(char *line);
//------------------ SINTAXIS FUNCTIONS
int syntax_check(t_token *tkn_lst);
//------------------ EXPANSION FUNCTIONS
char *ft_strjoin_free(char *s1, char *s2);
int is_valid_env_char(char c);
char *expand_value(t_token *token, t_env *env_lst);//*refactorizar muy larga
void expand_variables(t_token **tkn_lst, t_token *token, t_env *env_lst);
#endif