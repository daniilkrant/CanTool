#include <sys/cdefs.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>

#include <string>

class CanSend {

    public:
        static CanSend& getInstance()
        {
            static CanSend instance;
            return instance;
        }

        CanSend(CanSend const&) = delete;
        void operator=(CanSend const&)  = delete;
        int init(std::string name);
        int sendCommand(std::string canCommand);

    private:
        CanSend() {}

        std::string mInterfaceName;
        bool mIsInited{};
        int mSocket{};
        struct sockaddr_can mAddr;
        struct canfd_frame mFrame;
        struct ifreq mIfr;
};