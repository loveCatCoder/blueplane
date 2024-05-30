/*
  The sip library implements the Session Initiation Protocol (SIP -rfc3261-)
  Copyright (C) 2001,2002,2003,2004,2005,2006,2007 Aymeric MOIZARD jack@atosc.org
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdlib.h>
#include <stdio.h>

#include "sip_port.h"
#include "sip_list.h"

int sip_list_init(sip_list_t * li)
{
	if (li == NULL)
		return sip_BADPARAMETER;
	memset(li, 0, sizeof(sip_list_t));
	return sip_SUCCESS;		/* ok */
}

int sip_list_clone(const sip_list_t * src, sip_list_t * dst,
					int (*clone_func) (void *, void **))
{
	void *data;
	void *data2;
	sip_list_iterator_t iterator;

	for (data = sip_list_get_first((sip_list_t *) src, &iterator);
		 sip_list_iterator_has_elem(iterator);
		 data = sip_list_get_next(&iterator)) {
		int i;
		i = clone_func(data, &data2);
		if (i != 0)
			return i;
		sip_list_add(dst, data2, -1);
	}
	return sip_SUCCESS;
}

void sip_list_special_free(sip_list_t * li, void (*free_func) (void *))
{


	if (li == NULL)
		return;
	while (!sip_list_eol(li, 0)) {
		void *element;
		element = (void *) sip_list_get(li, 0);
		sip_list_remove(li, 0);
		if (free_func != NULL)
			free_func(element);
	}
}

void sip_list_ofchar_free(sip_list_t * li)
{
	int pos = 0;
	

	if (li == NULL)
		return;
	while (!sip_list_eol(li, pos)) {
		char *chain;
		chain = (char *) sip_list_get(li, pos);
		sip_list_remove(li, pos);
		sip_free(chain);
	}
}

int sip_list_size(const sip_list_t * li)
{
	if (li == NULL)
		return sip_BADPARAMETER;

	return li->nb_elt;
}

int sip_list_eol(const sip_list_t * li, int i)
{
	if (li == NULL)
		return sip_BADPARAMETER;
	if (i < li->nb_elt)
		return sip_SUCCESS;	/* not end of list */
	return 1;					/* end of list */
}

/* index starts from 0; */
int sip_list_add(sip_list_t * li, void *el, int pos)
{
	__node_t *ntmp;
	int i = 0;

	if (li == NULL)
		return sip_BADPARAMETER;

	if (li->nb_elt == 0) {

		li->node = (__node_t *) sip_malloc(sizeof(__node_t));
		if (li->node == NULL)
			return sip_NOMEM;
		li->node->element = el;
		li->node->next = NULL;
		li->nb_elt++;
		return li->nb_elt;
	}

	if (pos == -1 || pos >= li->nb_elt) {	/* insert at the end  */
		pos = li->nb_elt;
	}

	ntmp = li->node;			/* exist because nb_elt>0  */

	if (pos == 0) {				/* pos = 0 insert before first elt  */
		li->node = (__node_t *) sip_malloc(sizeof(__node_t));
		if (li->node == NULL) {
			/* leave the list unchanged */
			li->node = ntmp;
			return sip_NOMEM;
		}
		li->node->element = el;
		li->node->next = ntmp;
		li->nb_elt++;
		return li->nb_elt;
	}


	while (pos > i + 1) {
		i++;
		/* when pos>i next node exist  */
		ntmp = ntmp->next;
	}

	/* if pos==nb_elt next node does not exist  */
	if (pos == li->nb_elt) {
		ntmp->next = (__node_t *)sip_malloc(sizeof(__node_t));
		if (ntmp->next == NULL)
			return sip_NOMEM;	/* leave the list unchanged */
		ntmp = ntmp->next;
		ntmp->element = el;
		ntmp->next = NULL;
		li->nb_elt++;
		return li->nb_elt;
	}

	/* here pos==i so next node is where we want to insert new node */
	{
		__node_t *nextnode = ntmp->next;

		ntmp->next = (__node_t *)sip_malloc(sizeof(__node_t));
		if (ntmp->next == NULL) {
			/* leave the list unchanged */
			ntmp->next = nextnode;
			return sip_NOMEM;
		}
		ntmp = ntmp->next;
		ntmp->element = el;
		ntmp->next = nextnode;
		li->nb_elt++;
	}
	return li->nb_elt;
}

/* index starts from 0 */
void *sip_list_get(const sip_list_t * li, int pos)
{
	__node_t *ntmp;
	int i = 0;

	if (li == NULL)
		return NULL;

	if (pos < 0 || pos >= li->nb_elt)
		/* element does not exist */
		return NULL;


	ntmp = li->node;			/* exist because nb_elt>0 */

	while (pos > i) {
		i++;
		ntmp = ntmp->next;
	}
	return ntmp->element;
}

/* added by bennewit@cs.tu-berlin.de */
void *sip_list_get_first(sip_list_t * li, sip_list_iterator_t * iterator)
{
	if (0 >= li->nb_elt) {
		iterator->actual = 0;
		return sip_SUCCESS;
	}

	iterator->actual = li->node;
	iterator->prev = &li->node;
	iterator->li = li;
	iterator->pos = 0;

	return li->node->element;
}

/* added by bennewit@cs.tu-berlin.de */
void *sip_list_get_next(sip_list_iterator_t * iterator)
{
	iterator->prev = &(iterator->actual->next);
	iterator->actual = iterator->actual->next;
	++(iterator->pos);

	if (sip_list_iterator_has_elem(*iterator)) {
		return iterator->actual->element;
	}

	iterator->actual = 0;
	return sip_SUCCESS;
}

/* added by bennewit@cs.tu-berlin.de */
void *sip_list_iterator_remove(sip_list_iterator_t * iterator)
{
	if (sip_list_iterator_has_elem(*iterator)) {
		--(iterator->li->nb_elt);

		*(iterator->prev) = iterator->actual->next;

		sip_free(iterator->actual);
		iterator->actual = *(iterator->prev);
	}

	if (sip_list_iterator_has_elem(*iterator)) {
		return iterator->actual->element;
	}

	return sip_SUCCESS;
}

/* return -1 if failed */
int sip_list_remove(sip_list_t * li, int pos)
{

	__node_t *ntmp;
	int i = 0;

	if (li == NULL)
		return sip_BADPARAMETER;

	if (pos < 0 || pos >= li->nb_elt)
		/* element does not exist */
		return sip_UNDEFINED_ERROR;

	ntmp = li->node;			/* exist because nb_elt>0 */

	if ((pos == 0)) {			/* special case  */
		li->node = ntmp->next;
		li->nb_elt--;
		sip_free(ntmp);
		return li->nb_elt;
	}

	while (pos > i + 1) {
		i++;
		ntmp = ntmp->next;
	}

	/* insert new node */
	{
		__node_t *remnode;

		remnode = ntmp->next;
		ntmp->next = (ntmp->next)->next;
		sip_free(remnode);
		li->nb_elt--;
	}
	return li->nb_elt;
}