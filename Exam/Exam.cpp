#include <iostream>
#include <string>
using namespace std;

class Device {
public:
    int id;
    string name;
    int rated_power;
    Device() : id(0), name(""), rated_power(0) {}
    Device(int id, string name) {
        this->id = id;
        this->name = name;
        this->rated_power = 0;
    }

    virtual string kind() { return "Device"; }
    virtual void tick() {}
    virtual void describe() {}
    virtual void alerts() {}
};

class PoweredDevice : virtual public Device {
public:
    bool on;
    int energyWh;
    PoweredDevice() : on(0), energyWh(0), Device() {}
    PoweredDevice(int id, string name, int rated_power, bool on, int energyWh) : Device(id, name) {
        this->rated_power = rated_power;
        this->on = on;
        this->energyWh = energyWh;
    }

    void alerts() {
        if (on == false) {
            cout << "Device is turned off\n";
        }
    }

    string kind() {
        return "PoweredDevice";
    }
};
class SmartHome : public Device {
    Device* devices;
    int size;
    int nextId;
    string clock;
    string log;
public:
    SmartHome() : size(0), nextId(0), clock(""), log(""), Device() {}
    SmartHome(int id, string name, int nextId, string clock, string log) : Device(id, name) {
        this->size = 0;
        this->nextId = nextId;
        this->clock = clock;
        this->log = log;
    }

    void add(Device device) {
        Device* p = new Device[size + 1];
        for (int i = 0; i <= size; i++) {
            p[i] = devices[i];
        }

        p[size] = device;

        delete[] devices;
        devices = p;
        size++;
    }

    Device* search_by_id(int id) {
        for (int i = 0; i < size; i++) {
            if (devices[i].id == id) {
                return &devices[i];
            }
        }

        return nullptr;
    }

    void del(Device* device) {
        Device* p = new Device[size - 1];
        int pos = 0;

        for (int i = 0; i < size; i++) {
            if (device->id == devices[i].id) {
                pos = i;
            }
        }

        for (int i = 0; i < pos; i++) {
            p[i] = devices[i];
        }

        for (int i = pos + 1; i < size; i++) {
            p[i - 1] = devices[i];
        }


        delete[] devices;
        devices = p;
        size--;
    }

    void print_alerts() {
        for (int i = 0; i < size; i++) {
            devices[i].alerts();
        }
    }

    void print_report() {
        int sum = 0;
        for (int i = 0; i < size; i++) {
            sum += devices[i].rated_power;
            cout << "Device number " << i << ": " << devices[i].rated_power << "\n";
        }
        cout << "Rated power sum: " << sum << "\n";
        cout << "Log: " << log << "\n";
    }

    void print_devices() {
        for (int i = 0; i < size; i++) {
            cout << "Device: " << devices[i].name << "\n";
        }
    }

    Device* search_by_type(string type) {
        for (int i = 0; i < size; i++) {
            if (devices[i].kind() == type) {
                return &devices[i];
            }
        }

        return nullptr;
    }
};
class NetworkDevice : virtual public Device {
    int ip;
    bool online;
    int rssi;
public:
    NetworkDevice() : ip(0), rssi(0), online(0), Device() {}
    NetworkDevice(int id, string name, int ip, bool online, int rssi) : Device(id, name) {
        this->ip = ip;
        this->online = online;
        this->rssi = rssi;
    }

    bool isOnline() {
        return online;
    }

    void alerts() {
        if (rssi <= 2) {
            cout << "Signal is not strong\n";
        }

        if (isOnline() == false) {
            cout << "Device is offline\n";
        }
    }

    string kind() {
        return "NetworkDevice";
    }
};

class Idimmable {
public:
    virtual void setBrightness() {}
    virtual int getBrightness() { return 0; }
};

class SmartLamp : public PoweredDevice, NetworkDevice, Idimmable {
    int brightness;
public:
    SmartLamp() : brightness(0) {}
    SmartLamp(int id, string name, int rated_power, bool on, int energyWh, int ip, bool online, int rssi, int brightness) : PoweredDevice(id, name, rated_power, on, energyWh), NetworkDevice(id, name, ip, online, rssi) {
        this->brightness = brightness;
    }

    void setBrightness()override {
        cout << "Select brightness(1 - 100%): ";
        cin >> brightness;
    }

    int getBrightness()override { return brightness; }

    void dimmer() {
        if (isOnline()) {
            cout << "Select new brightness(1 - 100%): ";
            cin >> brightness;
        }
    }

    void alerts() {
        if (brightness < 10) {
            cout << "Brightness is low\n";
        }
    }

    string kind() {
        return "SmartLamp";
    }
};
class SmartPump : public PoweredDevice, NetworkDevice {
    int totalWater;
public:
    SmartPump(int id, string name, int rated_power, bool on, int energyWh, int ip, bool online, int rssi, int totalWater) : PoweredDevice(id, name, rated_power, on, energyWh), NetworkDevice(id, name, ip, online, rssi) {
        this->totalWater = totalWater;
    }
    void alerts() {
        if (totalWater == 0) {
            cout << "Water level is low\n";
        }
    }

    void irrigate() {
        if (isOnline()) {
            totalWater += 44;
            rated_power += 3;
        }
    }

    string kind() {
        return "SmartPump";
    }
};

class Sensor : public virtual NetworkDevice {
public:
    int lastValue;
    int* history = new int[10];
    Sensor() : lastValue(0), history(history), NetworkDevice() {}
    Sensor(int id, string name, int ip, bool online, int rssi, int lastValue) : NetworkDevice(id, name, ip, online, rssi) {
        this->lastValue = lastValue;
    }
    virtual void measure() {}
    virtual void unit() {}

    string kind() {
        return "Sensor";
    }
};

class BatteryPowered {
public:
    virtual void batteryLevel() {}
    virtual void charge() {}
    virtual bool isLow() { return false; }
};

class TempratureSensor : public Sensor {
    int temprature;
    string ambient;
public:
    TempratureSensor() : temprature(0), ambient(ambient), Sensor() {}
    TempratureSensor(int id, string name, int ip, bool online, int rssi, int lastValue, int temprature, string ambient) : Sensor(id, name, ip, online, rssi, lastValue) {
        this->temprature = temprature;
        this->ambient = ambient;
    }

    void measure()override {
        history = new int [10] { 3, -2, -1, -4, -1, 0, 2, 5, 4, 3 };
        lastValue = 3;
    }

    void unit()override {
        int sum = 0;
        for (int i = 0; i < 10; i++) {
            sum += history[i];
        }
        cout << "Average: " << sum / 10 << "\n" << "Last value: " << lastValue << "\n";
    }

    void alerts() {
        Sensor::alerts();
        if (lastValue < 10) {
            cout << "Temprature is low\n";
        }
        else if (lastValue > 25) {
            cout << "Temprature is too high\n";
        }
    }

    string kind() {
        return "TempratureSensor";
    }
};

class WirelessSensor : public Sensor, BatteryPowered {
    int battery;
    int drainPerTick;
public:
    WirelessSensor() : battery(0), drainPerTick(drainPerTick), Sensor() {}
    WirelessSensor(int id, string name, int ip, bool online, int rssi, int lastValue, int battery, int drainPerTick) : Sensor(id, name, ip, online, rssi, lastValue) {
        this->battery = battery;
        this->drainPerTick = drainPerTick;
    }

    void batteryLevel()override {
        cout << "Enter battery level: ";
        cin >> battery;
        if (battery <= 100 && battery >= 1) {
            cout << "Battery level: " << battery << "\n";
        }
    }

    void charge()override {
        cout << "Battery start charging!!!\n";
        battery = 100;
        cout << "Battery charging ends!!!\n";
    }

    bool isLow()override {
        bool islow = false;

        if (battery < 20) {
            cout << "Battery is low.\n";
            islow = true;
            charge();
            cout << "that was true, phone needed charge.\n";
        }
        return islow;
    }

    void alerts() {
        if (battery < 10) {
            cout << "Battery is low\n";
        }
    }

    string kind() {
        return "WirelessSensor";
    }
};

class SoilMostureSensor : public WirelessSensor {
    int moisture;
    int dry_rate;
    int threshold;
public:
    SoilMostureSensor() : moisture(0), dry_rate(0), threshold(0) {}
    SoilMostureSensor(int id, string name, int ip, bool online, int rssi, int lastValue, int battery, int drainPerTick, int moisture, int dry_rate, int threshold) : WirelessSensor(id, name, ip, online, rssi, lastValue, battery, drainPerTick) {
        this->moisture = moisture;
        this->dry_rate = dry_rate;
        this->threshold = threshold;
    }

    void measure()override {
        history = new int [10] { 23, 16, 31, 27, 22, 21, 13, 28, 41, 45 };
        lastValue = 45;
    }

    void unit()override {
        int sum = 0;
        for (int i = 0; i < 10; i++) {
            sum += history[i];
        }
        cout << "Average: " << sum / 10 << "\n" << "Last value: " << lastValue << "\n";
    }

    void alerts() {
        Sensor::alerts();
        if (lastValue < 10) {
            cout << "Moisture is low\n";
        }
        else if (lastValue > 90) {
            cout << "Moisture is too high\n";
        }
    }

    string kind() {
        return "SoliMostureSensor";
    }
};

int menu() {
    int choice;
    cout << "1 - Print devices\n";
    cout << "2 - Manage lamp\n";
    cout << "3 - Irrigation\n";
    cout << "4 - Alerts\n";
    cout << "5 - Reports\n";
    cout << "6 - Battery\n";
    cout << "7 - Add device\n";
    cout << "8 - Del device\n";
    cout << "Select choice: ";
    cin >> choice;
    return choice;
}

int main()
{
    bool is_running = true;
    int zaglushka;
    int type = 0, power = 0, on = 0, energy = 0, ip = 0, online = 0, rssi = 0, brightness = 0, water = 0, temperature = 0, battery = 0, drain = 0, moisture = 0, dry = 0, threshold = 0, value = 0;
    string ambient, name;

    SmartHome smart_home = SmartHome();
    SmartLamp* device;
    SmartPump* device2;
    BatteryPowered* device3;

    do {
        system("cls");
        int choice = menu();
        switch (choice) {
        case 1:
            smart_home.print_devices();
            cin >> zaglushka;
            break;
        case 2:
            device = dynamic_cast<SmartLamp*>(smart_home.search_by_type("SmartLamp"));
            device->setBrightness();

            break;
        case 3:
            device2 = dynamic_cast<SmartPump*>(smart_home.search_by_type("SmartPump"));
            device2->irrigate();

            break;
            break;
        case 4:
            smart_home.print_alerts();
            cin >> zaglushka;
            break;
        case 5:
            smart_home.print_report();
            cin >> zaglushka;
            break;
        case 6:
            device3 = dynamic_cast<BatteryPowered*>(smart_home.search_by_type("BatteryPowered"));
            device3->isLow();

            break;
        case 7:
            cout << "1 - SmartLamp\n";
            cout << "2 - SmartPump\n";
            cout << "3 - TempratureSensor\n";
            cout << "4 - WirelessSensor\n";
            cout << "5 - SoilMostureSensor\n";
            cout << "6 - PoweredDevice\n";
            cout << "7 - NetworkDevice\n";
            cout << "8 - Sensor\n";
            cout << "9 - Device\n";
            cout << "Device type: ";
            cin >> type;

            cout << "Name: ";
            getline(cin >> ws, name);

            if (type == 1 || type == 2 || type == 6) {
                cout << "Rated power: ";
                cin >> power;
                cout << "Is device on: ";
                cin >> on;
                cout << "Energy: ";
                cin >> energy;
            }
            if (type != 6 && type != 9) {
                cout << "IP: ";
                cin >> ip;
                cout << "Is device online: ";
                cin >> online;
                cout << "Signal: ";
                cin >> rssi;
            }

            switch (type) {
            case 1:
                cout << "Brightness (1-100): ";
                cin >> brightness;

                smart_home.add(SmartLamp(0, name, power, on, energy, ip, online, rssi, brightness));

                cout << "SmartLamp added.\n";
                break;
            case 2:
                cout << "Total water: ";
                cin >> water;

                smart_home.add(SmartPump(0, name, power, on, energy, ip, online, rssi, water));

                cout << "SmartPump added.\n";
                break;
            case 3:
                cout << "Temperature: ";
                cin >> temperature;
                cout << "Location: ";
                getline(cin >> ws, ambient);

                smart_home.add(TempratureSensor(0, name, ip, online, rssi, temperature, temperature, ambient));

                cout << "TempratureSensor added.\n";
                break;
            case 4:
                cout << "Last value: ";
                cin >> value;
                cout << "Battery (0-100):";
                cin >> battery;
                cout << "drain (0-100)";
                cin >> drain;

                smart_home.add(WirelessSensor(0, name, ip, online, rssi, value, battery, drain));

                cout << "WirelessSensor added.\n";
                break;
            case 5:
                cout << "Moisture: ";
                cin >> moisture;
                cout << "Battery: ";
                cin >> battery;
                cout << "Drain: ";
                cin >> drain;
                cout << "Dry: ";
                cin >> dry;
                cout << "Threshold: ";
                cin >> threshold;

                smart_home.add(SoilMostureSensor(0, name, ip, online, rssi,
                    moisture, battery, drain, moisture, dry, threshold));

                cout << "SoilMostureSensor added.\n";
                break;
            case 6:
                smart_home.add(PoweredDevice(0, name, power, on, energy));
                cout << "PoweredDevice added.\n";
                break;
            case 7:
                smart_home.add(NetworkDevice(0, name, ip, online, rssi));
                cout << "NetworkDevice added.\n";
                break;
            case 8:
                cout << "Last value: ";
                cin >> value;

                smart_home.add(Sensor(0, name, ip, online, rssi, value));
                cout << "Sensor added.\n";
                break;
            case 9:
                smart_home.add(Device(0, name));
                cout << "Device added.\n";
                break;
            }
            cin >> zaglushka;
            break;
        case 8:
            smart_home.print_devices();

            int id = 0;
            cout << "ID to delete: ";
            cin >> id;

            Device* found = smart_home.search_by_id(id);

            if (found != nullptr) {
                smart_home.del(found);
                cout << "Device deleted.\n";
            }
            else cout << "Device not found.\n";
            cin >> zaglushka;
            break;
        }
    } while (is_running);
}