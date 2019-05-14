#include "CanSend.hpp"
#include "lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

int CanSend::init(std::string name) {
    mInterfaceName = std::move(name);
    mIsInited = 1;

    return 1;
}

int CanSend::sendCommand(std::string canCommand) {
    if (!mIsInited) {
        return -1;
    } else {
        /* open socket */
	    if ((mSocket = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
	    	perror("socket");
	    	return 0;
	    }

	    strncpy(mIfr.ifr_name, mInterfaceName.c_str(), IFNAMSIZ - 1);
	    mIfr.ifr_name[IFNAMSIZ - 1] = '\0';
	    mIfr.ifr_ifindex = if_nametoindex(mIfr.ifr_name);
	    if (!mIfr.ifr_ifindex) {
	    	perror("if_nametoindex");
	    	return 0;
	    }

	    memset(&mAddr, 0, sizeof(mAddr));
	    mAddr.can_family = AF_CAN;
	    mAddr.can_ifindex = mIfr.ifr_ifindex;
        int required_mtu = 0;
        int mtu = 0;
        int enable_canfd = 1;

	    /* parse CAN mFrame */
	    required_mtu = parse_canframe(canCommand.c_str(), &mFrame);
	    if (!required_mtu){
	    	fprintf(stderr, "\nWrong CAN-mFrame format! Try:\n\n");
	    	fprintf(stderr, "    <can_id>#{data}            for 'classic' CAN 2.0 data frames\n");
	    	fprintf(stderr, "    <can_id>#R{len}            for 'classic' CAN 2.0 RTR frames\n");
	    	fprintf(stderr, "    <can_id>##<flags>{data}    for CAN FD frames\n\n");
	    	fprintf(stderr, "<can_id> can have 3 (SFF) or 8 (EFF) hex chars\n");
	    	fprintf(stderr, "{data} has 0..8 (0..64 CAN FD) ASCII hex-values (optionally");
	    	fprintf(stderr, " separated by '.')\n");
	    	fprintf(stderr, "{len} is an optional 0..8 value as RTR frames can contain a");
	    	fprintf(stderr, " valid dlc field\n");
	    	fprintf(stderr, "<flags> a single ASCII Hex value (0 .. F) which defines");
	    	fprintf(stderr, " canfd_frame.flags\n\n");
	    	fprintf(stderr, "e.g. 5A1#11.2233.44556677.88 / 123#DEADBEEF / 5AA# / ");
	    	fprintf(stderr, "123##1 / 213##311223344\n     1F334455#1122334455667788 / ");
	    	fprintf(stderr, "123#R / 00000123#R3\n\n");
	    	return 0;
	    }

	    if (required_mtu > CAN_MTU) {

	    	/* check if the mFrame fits into the CAN netdevice */
	    	if (ioctl(mSocket, SIOCGIFMTU, &mIfr) < 0) {
	    		perror("SIOCGIFMTU");
	    		return 0;
	    	}
	    	mtu = mIfr.ifr_mtu;

	    	if (mtu != CANFD_MTU) {
	    		printf("CAN interface is not CAN FD capable - sorry.\n");
	    		return 0;
	    	}

	    	/* interface is ok - try to switch the socket into CAN FD mode */
	    	if (setsockopt(mSocket, SOL_CAN_RAW, CAN_RAW_FD_FRAMES,
	    		       &enable_canfd, sizeof(enable_canfd))){
	    		printf("error when enabling CAN FD support\n");
	    		return 0;
	    	}

	    	/* ensure discrete CAN FD length values 0..8, 12, 16, 20, 24, 32, 64 */
	    	mFrame.len = can_dlc2len(can_len2dlc(mFrame.len));
	    }

	    /* disable default receive filter on this RAW socket */
	    /* This is obsolete as we do not read from the socket at all, but for */
	    /* this reason we can remove the receive list in the Kernel to save a */
	    /* little (really a very little!) CPU usage.                          */
	    setsockopt(mSocket, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);

	    if (bind(mSocket, (struct sockaddr *)&mAddr, sizeof(mAddr)) < 0) {
	    	perror("bind");
	    	return 0;
	    }

	    /* send mFrame */
	    if (write(mSocket, &mFrame, required_mtu) != required_mtu) {
	    	perror("write");
	    	return 0;
	    }

	    close(mSocket);

	    return 1;
        }
    
}