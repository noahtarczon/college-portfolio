#ifndef IPUBLISHER_H_
#define IPUBLISHER_H_

// #include <vector>
#include "ISubscriber.h"

  /**
   * @brief This class is a way for objects to be subscribed and 
   * unsubscribed to notifications related to entities 
   **/
class IPublisher {
 public:
    /**
     * @brief Destructor for IPublisher
     **/
    virtual ~IPublisher() {}

    /**
     * @brief Subscribe a subscriber to notifications
     * @param sub A pointer to the ISubscriber object being subscribed
    **/
    virtual void subscribe(ISubscriber* sub) = 0;

    /**
     * @brief Unsubscribe a subscriber from notifications
     * @param sub A pointer to the ISubscriber object being unsubscribed
    **/
    virtual void unsubscribe(ISubscriber* sub) = 0;

    /**
     * @brief Notify subscribers about events related to entities
     * @param entity A pointer to the IEntity object that the notifications are about
     * @param what A string describing the event related to the entity
    **/
    virtual void notify(IEntity *entity, const std::string& what) = 0;
};

#endif
