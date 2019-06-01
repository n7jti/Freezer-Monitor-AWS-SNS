// SNS API

//  SNS Code taken from the SMS Pet Food Dish.  
//  SMS Pet Food Dish example is copyright (c) 2013 Tony DiCola (tony@tonydicola.com)
//  and is released under an open source MIT license.  See details at:
//  http://opensource.org/licenses/MITlicense

void snsPublish(NTPClient &timeClient, const char* topic, const char* message);

class Sns : public Notification
{
public:
    Sns(Monitor &monitor, NTPClient &timeClient, const char* topic)
    virtual bool begin();
    void run();
protected:
    virtual void onRed(bool isNew);
    virtual void onYellow(bool isNew);
    virtual void onGreen(bool isNew); 
private:
    NTPClient &_timeClient;
    const char* _topic;
    bool _isAlerted; 
};

