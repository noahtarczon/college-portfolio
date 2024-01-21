#include "SpeedDecorator.h"



SpeedDecorator::SpeedDecorator(Drone* entity,
                            JsonObject obj) : IDroneDecorator(entity, obj) {
    this->drone = entity;
    this->revenue = 0.0;
    this->elapsedTime = 0.0;
}



bool SpeedDecorator::hasArrivedInTime() {
    return this->time > this->elapsedTime;
}


void SpeedDecorator::setTimeLimit() {
    double speed = this->drone->getSpeed();
    double limit = 0.0;
    if (speed == 30.0) {
        this->drone->setColor("white");
        limit = 300;
    } else if (speed == 60.0) {
        this->drone->setColor("blue");
        limit = 19;
    } else if (speed == 75.0) {
        this->drone->setColor("red");
        limit = 16;
    } else if (speed == 100.0) {
        this->drone->setColor("black");
        limit = 13;
    }
    if (this->isGold) {
        this->drone->setColor("gold");
    }
    this->time = limit;
}

void SpeedDecorator::update(double dt) {
    if (!this->drone->getAvailable()) {
        calcRevenue();
        this->elapsedTime += .01;
        this->drone->setSpeed(this->drone->getPackage()->getSpeed());
        if (!this->flag) {
            this->drone->notify(this->drone, "Scheduled");
        }
        if (this->time == -1) {
            this->setTimeLimit();
        }
        this->flag = true;
    }

    // Fixes random gold drone on start
    if (this->drone->getRevenue() > 10000) {
        std::cout << this->drone->getName()
        << " has "
        << this->drone->getRevenue()
        << " dollars"
        << std::endl;
        this->drone->setRevenue(0);
    }

    // Set Drone to Gold when Revenue cap is hit
    if (this->drone->getRevenue() > 100 && !this->isGold) {
        this->drone->setColor("gold");
        this->drone->notify(this->drone, "gold");
        this->isGold = true;
    }

    this->drone->update(dt);

    if (this->drone->getAvailable()) {
        if (this->flag) {
            std::cout << "TIME " << this->elapsedTime << std::endl;
            if (this->hasArrivedInTime()) {
                std::cout << this->drone->getName()<< " Arrived in time";
                std::cout << std::endl;
                float oldRev = this->drone->getRevenue();
                this->drone->tripRevenue = revenue;
                this->drone->setRevenue(oldRev + revenue);
                std::cout << this->drone->getName()
                << " made "
                << revenue
                << " dollars this trip."
                << std::endl;
                std::cout << this->drone->getName()
                << " has " << this->drone->getRevenue()
                << " dollars in total"
                << std::endl;
                this->drone->notify(this->drone, "onTime");
                revenue = 0;
                this->drone->tripRevenue = 0;
            } else {
                std::cout << this->drone->getName() << " NOT arrived in time";
                std::cout << std::endl;
                std::cout << this->drone->getName()
                << " made "
                << revenue
                << " dollars this trip."
                << std::endl;
                std::cout << this->drone->getName()
                << " has " << this->drone->getRevenue()
                << " dollars in total"
                << std::endl;
                revenue = 0;
                this->drone->tripRevenue = 0;
                this->drone->notify(this->drone, "notOnTime");
            }
            // resetting elapsedTime back to zero
            this->elapsedTime = 0;
            this->time = -1;
            this->flag = false;
        }
    }
}

void SpeedDecorator::calcRevenue() {
    double speed = drone->getSpeed();
    if (speed == 30.0) {
        revenue += .004;
    } else if (speed == 60.0) {
        revenue += .010;
    } else if (speed == 75.0) {
        revenue += .02;
    } else if (speed == 100.0) {
        revenue += .035;
    }
}

void SpeedDecorator::linkModel(SimulationModel* model) {
    this->drone->setModel(model);
}
