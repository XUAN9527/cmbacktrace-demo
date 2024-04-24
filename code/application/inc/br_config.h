#ifndef BR_CONFIG_H
#define BR_CONFIG_H

#include <stdarg.h>

//#define LIFE_SPAN_MODE				//注释掉时正常模式

#define APP_VERSION					(1)
#define APP_SUBVERSION				(0)
#define APP_REVISION				(1)

#ifndef LIFE_SPAN_MODE
	#define STRING_PROJECT_NAME			"CmBacktrace Demo"
#else
	#define STRING_PROJECT_NAME			"CmBacktrace Demo"
#endif
	
#define STRING_VERSION				"1.01"


#ifndef NULL
	#define NULL      				(0)
#endif

#define EC_TICK_PER_SECOND      	1000

//#define COUNT_DOWN_BASIC_TICKS		EC_TICK_PER_SECOND				/* TICK/1000ms */	
//#define SLEEP_DEFAULT_TICKS		(7000/COUNT_DOWN_BASIC_TICKS)		
//#define SLEEP_ABNORMAL_TICKS		(4000/COUNT_DOWN_BASIC_TICKS)		
//#define SLEEP_CHRMODE_TICKS		(1000/COUNT_DOWN_BASIC_TICKS)

//#define DEFAULT_WORK_TIME           (10000/COUNT_DOWN_BASIC_TICKS*60)			
//#define AGEING_ON_TIME              (10000/COUNT_DOWN_BASIC_TICKS*60)
//#define AGEING_OFF_TIME             (5000/COUNT_DOWN_BASIC_TICKS*60)
//#define AGEING_MODE_CNT				(5)
//#define TRIGGER_AGEMODE_CNT			(15)

/* my user defined*/
#define BSP_USING_PIN
#define BSP_USING_IWDG

#define BSP_USING_ON_CHIP_FLASH
#define RT_USING_LETTER_SHELL
#define BSP_USING_UART
#define BSP_USING_UART1
#define BSP_USING_UART1_DMATX
#define BSP_USING_UART3
//#define BSP_USING_UART3
//#define BSP_USING_UART4
//#define BSP_USING_UART5

#define BSP_USING_ADC
//#define BSP_USING_EXTI

#define BSP_USING_PWM
#define BSP_USING_PWM_TIM2
#define BSP_USING_PWM_TIM4
#define BSP_USING_PWM_TIM9

#define BSP_USING_PWM_INPUT
#define BSP_USING_INPUT_TIM4 //motor input:,TIM4_CH4,PB9

//#define BSP_USING_TIMER

//#define BSP_USING_SOFT_I2C

/* system option need to define and use */
#define RT_SECTION(x)               __attribute__((section(x)))
#define RT_UNUSED                   __attribute__((unused))
#define RT_USED                     __attribute__((used))
#define ALIGN(n)                    __attribute__((aligned(n)))
#define RT_WEAK                     __attribute__((weak))
#define rt_inline                   static inline

#if 0
typedef int (*init_fn_t)(void);
#define INIT_EXPORT(fn, level)                                                       \
            RT_USED const init_fn_t __rt_init_##fn RT_SECTION(".rti_fn." level) = fn
				
/* board init routines will be called in board_init() function */
#define INIT_BOARD_EXPORT(fn)           INIT_EXPORT(fn, "1")

/* pre/device/component/env/app init routines will be called in init_thread */
/* components pre-initialization (pure software initilization) */
#define INIT_PREV_EXPORT(fn)            INIT_EXPORT(fn, "2")
/* device initialization */
#define INIT_DEVICE_EXPORT(fn)          INIT_EXPORT(fn, "3")
/* components initialization (dfs, lwip, ...) */
#define INIT_COMPONENT_EXPORT(fn)       INIT_EXPORT(fn, "4")
/* environment initialization (mount disk, ...) */
#define INIT_ENV_EXPORT(fn)             INIT_EXPORT(fn, "5")
/* appliation initialization (rtgui application etc ...) */
#define INIT_APP_EXPORT(fn)             INIT_EXPORT(fn, "6")	

#endif		
			
/**
 * @ingroup BasicDef
 *
 * @def RT_ALIGN(size, align)
 * Return the most contiguous size aligned at specified width. RT_ALIGN(13, 4)
 * would return 16.
 */
#define RT_ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))

/**
 * @ingroup BasicDef
 *
 * @def RT_ALIGN_DOWN(size, align)
 * Return the down number of aligned at specified width. RT_ALIGN_DOWN(13, 4)
 * would return 12.
 */
#define RT_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

/**
 * @ingroup BasicDef
 *
 * @def RT_NULL
 * Similar as the \c NULL in C library.
 */
#define RT_NULL                         (0)

/**
 * Double List structure
 */
struct rt_list_node
{
    struct rt_list_node *next;                          /**< point to next node. */
    struct rt_list_node *prev;                          /**< point to prev node. */
};
typedef struct rt_list_node rt_list_t;                  /**< Type for lists. */

/**
 * rt_container_of - return the member address of ptr, if the type of ptr is the
 * struct type.
 */
#define rt_container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))


/**
 * @brief initialize a list object
 */
#define RT_LIST_OBJECT_INIT(object) { &(object), &(object) }

/**
 * @brief initialize a list
 *
 * @param l list to be initialized
 */
rt_inline void rt_list_init(rt_list_t *l)
{
    l->next = l->prev = l;
}

/**
 * @brief insert a node after a list
 *
 * @param l list to insert it
 * @param n new node to be inserted
 */
rt_inline void rt_list_insert_after(rt_list_t *l, rt_list_t *n)
{
    l->next->prev = n;
    n->next = l->next;

    l->next = n;
    n->prev = l;
}

/**
 * @brief insert a node before a list
 *
 * @param n new node to be inserted
 * @param l list to insert it
 */
rt_inline void rt_list_insert_before(rt_list_t *l, rt_list_t *n)
{
    l->prev->next = n;
    n->prev = l->prev;

    l->prev = n;
    n->next = l;
}

/**
 * @brief remove node from list.
 * @param n the node to remove from the list.
 */
rt_inline void rt_list_remove(rt_list_t *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;

    n->next = n->prev = n;
}

/**
 * @brief tests whether a list is empty
 * @param l the list to test.
 */
rt_inline int rt_list_isempty(const rt_list_t *l)
{
    return l->next == l;
}

/**
 * @brief get the list length
 * @param l the list to get.
 */
rt_inline unsigned int rt_list_len(const rt_list_t *l)
{
    unsigned int len = 0;
    const rt_list_t *p = l;
    while (p->next != l)
    {
        p = p->next;
        len ++;
    }

    return len;
}

/**
 * @brief get the struct for this entry
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define rt_list_entry(node, type, member) \
    rt_container_of(node, type, member)

/**
 * rt_list_for_each - iterate over a list
 * @pos:    the rt_list_t * to use as a loop cursor.
 * @head:   the head for your list.
 */
#define rt_list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * rt_list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:    the rt_list_t * to use as a loop cursor.
 * @n:      another rt_list_t * to use as temporary storage
 * @head:   the head for your list.
 */
#define rt_list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)

/**
 * rt_list_for_each_entry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define rt_list_for_each_entry(pos, head, member) \
    for (pos = rt_list_entry((head)->next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = rt_list_entry(pos->member.next, typeof(*pos), member))

/**
 * rt_list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:    the type * to use as a loop cursor.
 * @n:      another type * to use as temporary storage
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define rt_list_for_each_entry_safe(pos, n, head, member) \
    for (pos = rt_list_entry((head)->next, typeof(*pos), member), \
         n = rt_list_entry(pos->member.next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = n, n = rt_list_entry(n->member.next, typeof(*n), member))

/**
 * rt_list_first_entry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define rt_list_first_entry(ptr, type, member) \
    rt_list_entry((ptr)->next, type, member)

			
#endif

