#include "../includes/minirt_app.h"

/**
 * Print an error message to stderr
 */
void ft_print_error(const char *message)
{
    if (message)
        ft_putstr_fd((char *)message, STDERR_FILENO);
}

/**
 * Print an error message with additional details to stderr
 */
void ft_print_error_detail(const char *message, const char *detail)
{
    ft_print_error(message);
    if (detail)
        ft_putstr_fd((char *)detail, STDERR_FILENO);
}

/**
 * Print a number to stderr
 */
void ft_print_number(double num)
{
    char buffer[50];
    int i;
    int int_part;
    double frac_part;
    int temp;
    int digits;
    int j;
    int digit;
    
    i = 0;
    
    /* Handle negative numbers */
    if (num < 0)
    {
        ft_putchar_fd('-', STDERR_FILENO);
        num = -num;
    }
    
    /* Extract integer and fractional parts */
    int_part = (int)num;
    frac_part = num - int_part;
    
    /* Convert integer part to string */
    if (int_part == 0)
        buffer[i++] = '0';
    else
    {
        temp = int_part;
        digits = 0;
        
        /* Count number of digits */
        while (temp > 0)
        {
            temp /= 10;
            digits++;
        }
        
        /* Convert digits to chars */
        i = digits - 1;
        while (int_part > 0)
        {
            buffer[i--] = '0' + (int_part % 10);
            int_part /= 10;
        }
        i = digits;
    }
    
    /* Add decimal point and fraction if needed */
    if (frac_part > 0.0)
    {
        buffer[i++] = '.';
        
        /* Up to 6 decimal places */
        j = 0;
        while (j < 6 && frac_part > 0.0)
        {
            frac_part *= 10;
            digit = (int)frac_part;
            buffer[i++] = '0' + digit;
            frac_part -= digit;
            j++;
        }
    }
    
    buffer[i] = '\0';
    ft_putstr_fd(buffer, STDERR_FILENO);
}

/**
 * Print a vector to stderr (for error messages)
 */
void ft_print_vector(t_vec3 v)
{
    ft_putstr_fd("(", STDERR_FILENO);
    ft_print_number(v.x);
    ft_putstr_fd(", ", STDERR_FILENO);
    ft_print_number(v.y);
    ft_putstr_fd(", ", STDERR_FILENO);
    ft_print_number(v.z);
    ft_putstr_fd(")", STDERR_FILENO);
}
