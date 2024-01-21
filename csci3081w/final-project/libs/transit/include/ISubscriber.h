#ifndef ISUBSCRIBER_H_
#define ISUBSCRIBER_H_

#include <string>
#include "IEntity.h"

/**
 * @brief Class that allows objects to subscribe and be unsubscribed
 * from notifications
 *
 */
class ISubscriber {
 public:
    /**
     * @brief ISubscriber destructor
     *
     */
    virtual ~ISubscriber();
    /**
     * @brief Message that is being received from the IPublisher
     * 
     * @param entity A pointer to the IEntity object the message is about
     * @param what The string that describes the event related to the entity
     *
     */
    virtual void message(IEntity *entity, const std::string &what) = 0;
};

#endif
