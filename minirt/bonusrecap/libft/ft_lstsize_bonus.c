/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:44:25 by yrachidi          #+#    #+#             */
/*   Updated: 2024/11/02 11:12:18 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		len;
	t_list	*head;

	head = lst;
	len = 0;
	if (lst)
	{
		while (head)
		{
			len++;
			head = head->next;
		}
	}
	return (len);
}
