/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:46:55 by yrachidi          #+#    #+#             */
/*   Updated: 2024/11/02 11:02:46 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;
	t_list	*nextnode;

	if (!lst || !del)
		return ;
	tmp = *lst;
	while (tmp)
	{
		nextnode = tmp->next;
		ft_lstdelone(tmp, del);
		tmp = nextnode;
	}
	*lst = NULL;
}
