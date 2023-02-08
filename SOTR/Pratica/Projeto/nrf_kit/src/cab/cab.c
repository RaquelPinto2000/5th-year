#include "cab.h"

/**
 * @brief Description of a generic cab buffer
 * 
 */
typedef struct cab_buffer{
    unsigned int ref_count; // counter of nr of active references to this buffer, 
                            // if 0 -> free buffer
} cab_buffer;

/**
 * @brief CAB des
 * 
 */
typedef struct cab_desc
{
    char name[MAX_CAB_NAME];// cab's name
    unsigned int num_mes;   // max  number of message buffers
    unsigned int dim_mes;   // dimension in Bytes of a message
    char* initial;          // pointer to initial cab buffer

    //cab_buffer* cb_metadata;// pointer to the first struct holding buffers metadata
    //char* initialbuf;          // pointer to initial cab buffer

    cab_buffer* mrb;        // pointer to most recent written buffer
    bool has_message;       // flag to indicate that CAB has at least a message available

} cab_desc;

static int next_cab_free = 0;   //idx of next free cab position
static cab_desc* cabs[MAX_CABS] = { NULL };


CAB cab_create(char *name, unsigned int num_mes, unsigned int dim_mes, char* first) {
    if(next_cab_free == MAX_CABS){
        //All available cabs have been created
        return -1;
    }

    // create new cab descriptor
    cab_desc* cab = (cab_desc*) k_malloc(sizeof(cab_desc));

    // copy init data
    bytecpy(cab->name, name, MAX_CAB_NAME);
    cab->num_mes = num_mes;
    cab->dim_mes = dim_mes;
    
    // alloc required memory to save all the buffers and metadata
    cab->initial = k_malloc(num_mes * (dim_mes + sizeof(cab_buffer)));
    
    //cab->cb_metadata = (cab_buffer*)  k_malloc(num_mes * sizeof(cab_buffer));
    //cab->initialbuf = k_malloc(num_mes * dim_mes);
    
    // init all buffers
    for(int i=0; i<num_mes;i++) {
        cab_buffer* cb_ptr = (cab_buffer*) (cab->initial + i*( sizeof(cab_buffer) + dim_mes ));
        cb_ptr->ref_count=0;
    }
    /*
    for(int i=0; i<num_mes;i++) {
        (cab->cb_metadata + i)->ref_count = 0;
    }*/
    
    // store initial msg
    if (first != NULL) {
        bytecpy(cab->initial+sizeof(cab_buffer), first, cab->dim_mes);
        cab->mrb= (cab_buffer*) cab->initial;
        cab->mrb->ref_count++;
        cab->has_message=true;
    } else {
        cab->mrb = NULL;
        cab->has_message=false;
    }

    cabs[next_cab_free] = cab;
    return next_cab_free++;
}

char* cab_reserve(CAB id){
    // check if given cab haas been initialized
    if(cabs[id] == NULL) {
        return NULL;
    }

    cab_desc* cab = cabs[id];
    cab_buffer* cb_free_ptr = NULL;

    // Critical Section: start
    int key = irq_lock();

    for(int i=0;i<cab->num_mes;i++) {
        cab_buffer* cb_ptr =  (cab_buffer*) (cab->initial + i*(sizeof(cab_buffer) + cab->dim_mes));
        if(cb_ptr->ref_count == 0) {
            cb_free_ptr = cb_ptr;
            break;
        }
    }

    if(cb_free_ptr == NULL) {
        return NULL;
    }

    // set in use flag
    cb_free_ptr->ref_count=1;
    
    // Critical Section: end
    irq_unlock(key);

    return ((char*) cb_free_ptr) + sizeof(cab_buffer);
}

int cab_putmes(CAB id, char *pbuf) {
    // check if given cab haas been initialized
    if(cabs[id] == NULL) {
        return -1;
    }

    cab_desc* cab = cabs[id];
    cab_buffer* cb_ptr = (cab_buffer*) (pbuf - sizeof(cab_buffer));
    // Critical Section: start
    int key = irq_lock();

    cab->mrb = cb_ptr;
    cab->has_message = true;
    
    // release in use flag
    cb_ptr->ref_count = 0;
    // Critical Section: end
    irq_unlock(key);

    return 0;
}

bool cab_hasmes(CAB id) {
    if(cabs[id] == NULL) {
        return false;
    }

    return cabs[id]->has_message;
}

char* cab_getmes(CAB id) {
    // check if given cab haas been initialized
    if(cabs[id] == NULL) {
        return NULL;
    }

    // Critical Section: start
    int key = irq_lock();

    cab_buffer* mrb = cabs[id]->mrb;
    mrb->ref_count++;
    // Critical Section: end
    irq_unlock(key);


    return (char*) mrb + sizeof(cab_buffer);
}

int cab_unget(CAB id, char* pun_mes){
    // check if given cab haas been initialized
    if(cabs[id] == NULL) {
        return -1;
    }
    
    ((cab_buffer*) (pun_mes - sizeof(cab_buffer)))->ref_count--;

    return 0;
}

int cab_delete(CAB id){
    if(cabs[id] == NULL) {
        return 0;
    }

    cab_desc* cab = cabs[id];
    k_free(cab->initial);
    k_free(cab);
    cabs[id] = NULL;
    return 0;
}
