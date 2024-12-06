#include<iostream>
#include<vector>
#include<cmath>
#include<limits>
#include<map>
#include<algorithm>
using namespace std;

//Represent a location
struct Location{
    string name;
    double x,y;
    double distanceTo(const Location& other)const{
        return sqrt(pow(x-other.x,2)+pow(y-other.y,2));
    }
};
//Structure for driver
struct Driver{
    int id;
    Location location;
    bool available;
    string currentZone;
};
//For Ride req
struct RideRequest{
    int id;
    Location pickup;
    Location dropoff;
};
//For a Zone
struct Zone{
    string name;
    Location center;
    int demand;
    int driverCount;
};
//Greedy approach
Driver *findNearestDriver(vector<Driver>& drivers,const Location& pickup){
    double minDistance=numeric_limits<double>::max();
    Driver* nearestDriver=nullptr;
    for(auto& driver:drivers){
        if(driver.available){
            double dist=driver.location.distanceTo(pickup);
            if(dist<minDistance){
                minDistance=dist;
                nearestDriver= &driver;
            }
        }
    }
    return nearestDriver;
}
//Shortest path
double findShortestPath(const Location& start,const Location& end){
    return start.distanceTo(end);//assume direct path
}
//Dynamic fleet management
void dynamicFleetManagement(vector<Driver>& drivers,vector<Zone>& zones){
    //recalculate driver counts in each zone
    for(auto& zone:zones){
        zone.driverCount=0;
    }
    for(const auto& driver:drivers){
        if(!driver.available)
        continue;//ignore unavailable driver
        for(auto& zone:zones){
            if(driver.currentZone==zone.name){
                zone.driverCount++;
                break;
            }
        }
    }
    //Short zone by demand
    sort(zones.begin(),zones.end(),[](const Zone& a,const Zone& b){
        if(b.driverCount==0)
        return true;
        return(double)a.demand/max(1,a.driverCount)>(double)b.demand/max(1,b.driverCount);
    });
    cout<<"Dynamic Fleet Mangement:\nAnalyzing demand and driver allocation....\n";
    for(const auto& zone:zones){
        cout<<"Zone "<< zone.name << " ; demand= "<< zone.demand << ", Drivers= "<< zone.driverCount << "\n";
    }
    for(auto& driver:drivers){
        if(!driver.available)
        continue;//only concider available driver
        for(auto& zone:zones){
            if(zone.driverCount<zone.demand){
                string newZone=zone.name;
                if(driver.currentZone !=newZone){
                    cout<<"Driver "<< driver.id <<" moved from zone " << driver.currentZone <<" to zone "<<newZone<<endl;
                    driver.currentZone=newZone;
                    zone.driverCount++;
                    break;
                }
            }
        }
    }

}
//Back tracking
bool reassignDriver(map<int,RideRequest>& ongoingRides,vector<Driver>& drivers,int rideId){
    if(ongoingRides.find(rideId) !=ongoingRides.end()){
        RideRequest request=ongoingRides[rideId];
        cout<< "Ride "<< rideId <<" is canceled. Attempting to reassign...\n";
        Driver* newDriver=findNearestDriver(drivers, request.pickup);
        if(newDriver){
            cout<<"Finding nearest available driver for Ride "<< rideId <<" (Pickup: "<< request.pickup.x <<", "<< request.pickup.y << ")...\n";
            double dist=request.pickup.distanceTo(newDriver->location);
            cout<<"Nearest Driver: Driver "<< newDriver->id << " (Distance to pickup= "<< dist << ")...\n";
            cout<<"Ride "<< rideId << " reassigned to Driver " << newDriver->id <<endl;
            newDriver->available=false;
            newDriver->location=request.dropoff;
            ongoingRides.erase(rideId);
            return true;
        }else{
            cout<<"Failed to reassign ride "<< rideId <<" due to no available drivers.\n";
            return false;
        }
    }
    return false;
}
int main(){
    //Zone
    vector<Zone> zones={
        {"Zone A",{"Center A", 2.0, 3.0}, 5, 0},
        {"Zone B",{"Center B", 5.0, 1.0}, 8, 0},
        {"Zone C",{"Center C", 8.0, 6.0}, 2, 0},
    };
    //Driver
    vector<Driver> drivers={
        {1,{"Zone A", 2.0, 3.0}, true, "Zone A"},
        {2,{"Zone B", 5.0, 1.0}, true, "Zone B"},
        {3,{"Zone C", 8.0, 6.0}, true, "Zone C"},
        {4,{"Zone A", 2.1, 3.1}, true, "Zone A"},
        {5,{"Zone B", 4.9, 1.2}, false, "Zone B"},
    };
    //Ride request
    vector<RideRequest> requests={
        {1,{"Pickup 1", 3.0, 3.0},{"Dropoff 1", 7.0, 8.0}},
        {2,{"Pickup 2", 6.0, 2.0},{"Dropoff 2", 10.0, 10.0}},
    };
    map<int, RideRequest> ongoingRides;

    //initial status
    cout<<"Ride Sharing System Simulation\n\nInitial Driver & Zone Status:\n";
    for(const auto& driver:drivers){
        cout<<"Driver "<< driver.id <<": "<<(driver.available?"Available":"Unavailable")
        <<" in "<< driver.currentZone <<" at (" << driver.location.x <<", "<< driver.location.y << ")\n";
    }
    for(const auto& zone:zones){
        cout<<"Zone "<< zone.name << ": Demand= "<< zone.demand << ", Drivers= "<< zone.driverCount << "\n";
    }
    cout<<"\n---------------------------------------------------------------\n";
    //Ride processing
    for(const auto& request:requests){
        cout<<"Processing Ride Request "<< request.id <<endl;
        Driver* driver=findNearestDriver(drivers,request.pickup);
        if(driver){
            cout<<"Driver "<< driver->id << " assigned to Ride " << request.id <<endl;
            driver->available=false;
            ongoingRides[request.id]=request;
            //calculate shortest path
            double pathLength = findShortestPath(driver->location, request.pickup) + findShortestPath(request.pickup, request.dropoff);
            cout << "Estimated distance: \n"
            << " - From Driver " << driver->id << " to pickup: "
            << driver->location.distanceTo(request.pickup) << "\n"
            << " - From pickup to dropoff: "
            << request.pickup.distanceTo(request.dropoff) << "\n";
            cout<<"Total trip distance: " << pathLength << "\n\n";
            driver->location = request.dropoff;
        }else{
            cout<<"No available driver to Ride "<< request.id <<endl;
        }
        cout<<"---------------------------------------------------------------\n";
    }
    //Perform Dynamic fleet management
    dynamicFleetManagement(drivers, zones);
    cout<<"\nFinal Driver Allocation After Re-distribution: \n";
    for(const auto& driver:drivers){
        cout<<"Driver "<< driver.id << " : "
        << (driver.available ? "Available" : "Unavailable")
        <<" in "<< driver.currentZone <<endl;
    }
    //Simulating ride cancellation
    cout<<"\nSimulating Ride Cancellation\n";
    int rideToCancel=1;
    if(reassignDriver(ongoingRides, drivers, rideToCancel)) {
    cout<< "Ride cancellation managed successfully.\n";
    }else{
    cout << "Ride cancellation could not be managed.\n";
    }
    cout<<"\n-----------------------------------------------------------------\n";
    cout<<"Final Status of Drivers:\n";
    for(const auto& driver:drivers){
        cout<<"Driver "<< driver.id << " : "
        << (driver.available ? "Available" : "Unavailable")
        << " in "<< driver.currentZone <<endl;
    }
    cout<<"\nFinal Status of Zones:\n";
    for(const auto& zone:zones){
        cout<<"Zone " << zone.name << ": Demand = " << zone.demand <<", Drivers = "<< zone.driverCount << "\n";
    }
  return 0;  
}