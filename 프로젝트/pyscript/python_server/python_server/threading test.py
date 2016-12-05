import threading

import arm_m3_kit_server 
import traceback

if __name__ == "__main__":
	th = threading.Thread(arm_m3_kit_server.arm_m3_kit_server.main())
	try:
		th.start();
	except:
		traceback.print_exc(file = sys.stdout)
