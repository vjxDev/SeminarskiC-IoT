import { mount, StartClient } from "solid-start/entry-client";

import { initBLEonClient } from "~/store/BLEStore";

mount(() => <StartClient />, document);
initBLEonClient();
