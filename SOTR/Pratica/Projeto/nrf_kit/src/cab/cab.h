/**
 * @file cab.h
 * @author João Martins (joaofmartins@ua.pt); Raquel Milheiro (raq.milh@ua.pt)
 * @brief Image processing algorithms for SOTR 22 final project.
 * @version 0.1
 * @date 2022-12-10 
 * 
 * Influenced by the HARTIK header file.
 * 
*/

#ifndef __CAB_H__
#define __CAB_H__
#include <kernel.h>

#define MAX_CABS 4     // Max number of cabs that can be allocated
#define MAX_CAB_NAME 20 // Max size in bytes/chars for the cab name


typedef int CAB;

/**
 * @brief Initializes a CAB.
 * 
 * @param name arbitrary string.
 * @param num_mes Number of messages that the CAB may contain simultaneously.
 * @param dim_mes dimension (in Bytes) of the message type.
 * @param first pointer to the initial message contained in the CAB.
 * @return CAB - id for the newly created CAB.
 */
CAB cab_create(char* name, unsigned int num_mes, unsigned int dim_mes, char* first);

/**
 * @brief Returns a pointer to a free buffer of the given CAB.
 * 
 * @param id CAB id that owns the buffer.
 * @return char* pointer to the message buffer, -1 if the given CAB hasnt been initialized.
 */
char* cab_reserve(CAB id);

/**
 * @brief Puts message buffer back in the CAB as the most recent message.
 * 
 * @param id CAB id of the buffer owner.
 * @param pbuf pointer to the message buffer.
 * @return int 0 if sucess.
 */
int cab_putmes(CAB id, char* pbuf);

/**
 * @brief Indicates that the CAB has a message available.
 * 
 * @param id 
 * @return true if there is at least a message available.
 * @return false if no message has been stored in the given CAB.
 */
bool cab_hasmes(CAB id);

/**
 * @brief Returns poiter to the buffer with the most recent message.
 * 
 * @param id CAB that owns the message.
 * @return char* pointer to the message buffer, NULL if error.
 */
char* cab_getmes(CAB id);

/**
 * @brief Signal the given CAB that buffer no longer needed by calling task.
 * 
 * @param id CAB id
 * @param pun_mes pointer to message buffer.
 * @return int 0 if sucess.
 */
int cab_unget(CAB id, char* pun_mes);

/**
 * @brief Deletes CAB and releases the memory.
 * 
 * @param id
 * @return int 0 if sucess.
 */
int cab_delete(CAB id);

#endif