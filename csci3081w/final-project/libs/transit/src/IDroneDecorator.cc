#include "IDroneDecorator.h"

IDroneDecorator::IDroneDecorator(Drone* entity,
                                JsonObject obj) : IEntity(obj) {}
