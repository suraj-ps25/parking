#include <iostream>
#include <string>
#include <array>
#include <algorithm>
#include <vector>
#include <map>
#include <utility>
#include <cmath>
#include <set>
#include <climits>
#include <queue>
#include <iomanip>
#include <numeric>
#include <stack>
#include <chrono>

using namespace std ;

// ticket -> parking lot -> level -> free spot -> parked

// praking lot (levels)
// levels -> spots
// spots -> size


enum class VehicleType {Car, Bike, Truck};
enum class SpotSize {Small , medium , large};

class Vehicle;
class ParkingSpot;

class Ticket {
    inline static int idCount = 0;
    int ticketId;
    ParkingSpot* spot;
    chrono::time_point<chrono::system_clock> time;
    Vehicle* vehicle;

    public :
        Ticket(Vehicle* v, ParkingSpot* s)  {
            vehicle = v;
            spot = s;
            idCount++;
            ticketId = idCount;
            time = chrono::system_clock::now();
        }

        ParkingSpot* getSpot() {
            return spot;
        }

        int getTicketId() {
            return ticketId;
        }

};

class Vehicle {
    string NumberPlate;
    VehicleType type;

    public : 
        Vehicle(string plate, VehicleType t) {
            NumberPlate = plate;
            type = t;
        }
        VehicleType getType() {
            return type;
        }
        string getNumberPLate() {
            return NumberPlate;
        }
};

class Bike : public Vehicle {
    public :
        Bike(string plate) : Vehicle(plate, VehicleType::Bike){}
};


class Car : public Vehicle {
    public :
        Car(string plate) : Vehicle(plate, VehicleType::Car){}
};


class Truck : public Vehicle {
    public :
        Truck(string plate) : Vehicle(plate, VehicleType::Truck){}
};

class ParkingSpot {
    // int spotId;
    SpotSize size;
    Vehicle* vehicle;
    bool isFree;

    public : 
        ParkingSpot(int id, SpotSize s) {
            // spotId = id;
            size = s;
            isFree = true;
            vehicle = NULL;
        }

        bool canFitVehicle(Vehicle* v) {
            if(!isFree) return 0;

            VehicleType vt = v->getType();

            if(vt == VehicleType::Bike) return 1;
            if(vt == VehicleType::Car && (size != SpotSize::Small)) return 1;
            if(vt == VehicleType::Truck && size == SpotSize::large) return 1;

            return 0;
        }

        void assignVehicle(Vehicle* v) {
            isFree = 0;
            vehicle = v;
        }
        void removeVehicle() {
            isFree = 1;
            vehicle = NULL;
        }

        bool getIsFree() {
            return isFree;
        }

        SpotSize getSize() {
            return size;
        }
        // bool getSpotId() {
        //     return spotId;
        // }
};

// int Ticket::idCount = 0;

class Level {
    int floorNumber;
    vector<ParkingSpot*> spots;

    public : 
        Level(int num, int numOfSpots) {
            cout<<"Creating level with floor NUmber "<<num<<" with "<<numOfSpots<<" spots in this level"<<endl;
            floorNumber = num;
            for(int i=1; i<=numOfSpots; i++) {
                SpotSize size = (i<=(numOfSpots/3) ? SpotSize::Small
                     : (i<=2*(numOfSpots/3)) ? SpotSize ::medium : SpotSize::large);
                spots.push_back(new ParkingSpot(i,size));
            }
        }

        Ticket* ParkVehicle(Vehicle* v) {
            cout<<"started the process to park vehicle with type"<<endl;
            for(auto spot : spots) {
                if(spot->canFitVehicle(v)) {
                    cout<<"parked it"<<endl;
                    spot->assignVehicle(v);
                    return new Ticket(v,spot);
                }
            }
            return NULL;
        }

        bool UnParkVehicle(Ticket* t) {
            ParkingSpot* ps = t->getSpot();
            
            if(ps != NULL && ps->getIsFree() == false) {
                ps->removeVehicle();
                return 1;
            }
            return 0;
        }

        void printAllSpots() {
        for (int i = 0; i < spots.size(); ++i) {
            string spotSize;
            switch (spots[i]->getSize()) {
                case SpotSize::Small : spotSize = "Small"; break;
                case SpotSize::medium : spotSize = "Medium"; break;
                case SpotSize::large : spotSize = "Large"; break;
            }

            cout << "Spot " << i + 1 << ": " << spotSize
                 << ", " << (spots[i]->getIsFree() ? "Free" : "Occupied") << endl;
        }
    }
};

class ParkingLot {
    vector<Level*> levels;

    public :
        ParkingLot(int floor, int numOfSpots) {
            cout<<"making parking lot with "<<floor<<" floors and "<<numOfSpots<<" in each floor"<<endl;
            for(int i=1; i<=floor; i++) {
             levels.push_back(new Level(i,numOfSpots));
            }
        }

        Ticket* ParkVehicle(Vehicle* v) {
            for(auto level : levels) {
                Ticket* t = level->ParkVehicle(v);
                if(t != NULL) {
                    cout<<"parket vehicle with ticket id->"<<t->getTicketId()<<endl;
                    return t;
                }
            }
            cout<<"Parking FUll for Vehicle type";
        }

        void UnParkVehicle(Ticket* ticket) {
            ParkingSpot* ps = ticket->getSpot();
            ps->removeVehicle();
        }

        void printAllSpots() {
        cout << "Printing all spots in all levels of the parking lot:\n";
        for (int i = 0; i < levels.size(); ++i) {
            cout << "Level " << i + 1 << ":\n";
            levels[i]->printAllSpots(); 
        }
    }
};

int main(){

    ParkingLot lot(3,3);
    Vehicle* car = new Car("KA-01-HH-1234");

    Ticket* ticket = lot.ParkVehicle(car);

    lot.printAllSpots();

    if (ticket) {
        cout << "Car parked with ticket ID: " << ticket->getTicketId() << endl;
    }

    lot.UnParkVehicle(ticket);
    cout << "Car unparked." << endl;

    lot.printAllSpots();


return 0;
}