/**
 * @file cab_msg.h
 * @author João Martins (joaofmartins@ua.pt); Raquel Milheiro (raq.milh@ua.pt)
 * @brief Header file with the CAB messages definitions.
 * @version 0.1
 * @date 2022-12-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CAB_MSGs_H__
#define __CAB_MSGs_H__

#include "imageProcessing/imageProcessing.h"

/**
 * @brief Message with the image data to be analized.
 * 
 */
typedef struct cab_msg_image
{
    char image_data[IMGWIDTH][IMGWIDTH];
} cab_msg_image;

/**
 * @brief Message for the 'detected objects in the CSA' flag.
 * 
 */
typedef struct cab_msg_near_objects
{
    bool near_objects;
} cab_msg_near_objects;

/**
 * @brief Message for the number of objects detected.
 * 
 */
typedef struct cab_msg_num_objects
{
    int num_objects;
} cab_msg_num_objects;

/**
 * @brief Message with guideline details
 * 
 */
typedef struct cab_msg_guideline
{
    int16_t pos;
	float angle;
} cab_msg_guideline;

#endif