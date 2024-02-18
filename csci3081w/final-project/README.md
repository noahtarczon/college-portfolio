# 3081 Final Project - Team-001-22  🚁 📦

### Team Members:
- Javier A
- Tristan K
- Morgan N
- Noah Tarczon

## Project Overview

The project is about a drone simulation system over the University of Minnesota Twin Cities campus. The drones deliver packages to robots, and this is scheduled through an HTML page that allows a user to choose locations the drone will pick up and drop a package off at. 

## Youtube Walkthrough
https://youtu.be/i78_qh8ion0

## How to Run the Simulation

### Method: SSHing into a CSE Lab Machine
Steps:
1. ```git clone https://github.umn.edu/umn-csci-3081-f23/team-001-22-hw4.git```
1. ```cd team001-22-hw4```
1. Run ```make -j``` in the terminal. 
1. Run ```./build/bin/transit_service 8081 apps/transit_service/web/``` in the terminal. 
1. View the simulation in ```localhost:8081```

### Method: Using Docker
Steps:
1. ```docker pull morgannakonechny/drone_sim```
1. ```docker run --rm -it -p 8081:8081 morgannakonechny/drone_sim```

## What does the simulation do specifically?
The simulation has many different aspects to it. Deliveries can be scheduled through an HTML page. This page allows a user to enter a name for the delivery, pick a routing algorithm for the drone to use, and the extension allows them to pick a speed for delivery as well. The user can also add new entities such as: 
- Drones
- Humans

Drones get triggered to move to packages based on the user input from the HTML page. When a drone moves to pick up the package, it beelines to the package. Then, via the search algorithm that was chosen, the package is picked up and moves with the drone until it is dropped off at the specified location where a robot is waiting. The drones will do a celebration, either a spin or an up and down movement, when the package is delivered to the proper spot. 

There are also two more entities that include a helicopter and a human. These move on their own and cannot be controlled, but the user can add more humans if they want to.

# New Features

## 1. Delivery Methods
### What does it do?
The first feature is varying speeds of delivery. This allows the user to change the speed their package is delivered with if they want their package to arrive quicker. There are four options: Standard, Priority, Priority Express, and First-Class. Standard is the slowest speed, and this is the speed the drone was set with before the extension. From there, each new option increases in speed and also becomes increasingly expensive. The user chooses speed in a drop down menu on the HTML form when they are scheduling their delivery, and from there the drone will run at the corresponding speed. Each of these four speeds also has a corresponding color that the drone will turn to signify which speed it is going. The drone makes money from these deliveries, and it keeps track of the revenue it makes. Time is also factored in, and for each drone speed there is a certain amount of time that they have to deliver the package in. If the drone does not make it in time the drone does not make the money, and therefore receives $0 for that trip. Once the drone makes a total of $100 it will turn gold to signify it has reached gold status, and it will stay gold regardless of the speed it goes. 
### Why is it significantly interesting?
This is significantly interesting because from a business perspective it allows accessibility for different types of user needs in the system. If a user needs a package delivered in a certain amount of time or a fast delivery in general they now have that option. If this is not relevant to them, they can choose a cheaper option or the "standard" speed option, which has no time guarantee. This feature reaches a larger audience and accounts for the needs of existing users. In addition, it allows the business to make more money if there is a demand for faster deliveries. Keeping track of the revenue the drones make allows the business to see which areas on the map make the most money and which areas are more likely to be willing to pay more for a delivery. This is important information that allows the business to see how their services are functioning and how to improve or expand. 
### How does it add to the existing work? 
This extension adds more speeds to the drone, where as before there was only one speed. This is done by adding a dropdown in the HTML. The user response from the dropdown is then transferred through the ```schedule.html``` file to alter the speed upon decoration of the drone. It also changes the color of the drones themselves to convey information about its speeds and revenue statuses. 
### Which design pattern did you choose to implement it and why?
This extension was implemented with a decorator design pattern. This was the best option because it gave us the flexibility of adding additional features to a drone without having to change or create a different drone. This is especially true because we are giving the drone a new speed based on the user's chosen tier. We wanted the drone to persist the entire time the simulation is running, while also having the ability to tweak the functionality. With other design patterns we would need to make bigger changes and the code would not be as extendable. 
### Instruction to use this new feature:
This new feature can be used by running the simulation as normal and then going to the scheduling page. From there the user should choose the speed with which they want their package delivered in the dropdown. Everything else will happen without user interaction. In addition, it is important to note that the slider bar for speed was taken out. This is because it would change the time step that was used for calculations and cause weird jumps + overflow/underflow errors in calculations when the simulation model was first loaded. 

## 2. Delivery + Drone Notifications
### What does it do?
The second feature is a notification system that gives users real-time updates on their package and the status of the drones. It updates when the package is picked up and when it is successfully delivered. It also provides updates on if the package arrived in time based on the revenue amount displayed. If the drone did not arrive in time, it will display an error message and display that the drone made $0, meaning the user was "refunded". If the drone arrived within the alloted time limit the notification system will display that, as well as the revenue the drone made. The notification system also displays the total amount of money each drone has made, as well as their color and when a drone has reached gold status.
### Why is it significantly interesting?
This is significantly interesting because notifications enhance the users experience. The communication about packages allows the user to stay updated with the delivery process which can be very valuable. This system also allows users to be informed about if their package is taking longer than anticipated. If the delivery is not up to customer standards, it also notifies the customer about a refund. 
### How does it add to the existing work? 
This adds to the existing work by allowing users to receive real-time updates about the functions of the simulation. Our notification system allows for extendability by adding more attributes about the entities to create more specific messages. These notifications to the user are also logging features that were previously implemented before the extension.
### Which design pattern did you choose to implement it and why?
This extension was implemented with the observer design pattern. This is because we wanted a user to be able to see updates on their packages via subscription. 
### Instruction to use this new feature:
The notification system does not require user interaction because the user is automatically subscribed to notifications when they schedule deliveries. The notifications will automatically show up on the left side of the screen based on the drones movements and details as well as the package being delivered. Some of this will be determined by the user when they schedule the delivery in the HTML page, such as the speed because this changes the drones color. The user can interact with the notification bar by scrolling down once the notifications reach the bottom of the page. 

## Sprint Retrospective
We organized our development process into 2 sprints, breaking the 2 extensions into separate sprints. We planned for and completed organization tasks before working on the first extension. This included rough drafts of UML, separating tasks between members, and planning team meetings. We were able to complete our first extension, the Speed Decorator, by our estimated Sprint 1 end date of December 6th. We then moved on to our second extension, the Observer, and final tasks such as Doxygen, Code Styling, Documentation, and the filming of the Youtube video. 

We did a great job of separating our work into 2 reasonable sprints, but this was challenging at first to plan as we didn't know the alloted time tasks would take before starting to code. It was difficult to split up tasks for this same reason, as we didn't know if a single task was equal to another in time length and effort. We did most all of our coding in a group setting, all brainstorming together instead of splitting up coding tasks and integrating them. We believe if we would have split up more tasks to be individual responsiblities, it would have been more difficult to keep track of what was completed. The only con to this was that in a group setting, one person was coding at a time, causing one person to be more knowledgable of the codebase for that specific functionality being added. The sprints helped us understand where we were at in the development process and kept a more accurate timeline of our extension. 
