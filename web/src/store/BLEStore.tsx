import { createStore } from "solid-js/store";

const serviceUUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";

// All UUIDs
export enum DATA_UUIDS {
  "WIFI_SSID" = "beb5483e-36e1-4688-b7f5-ea07361b26a8",
  "WIFI_PASSWORD" = "beb5483e-36e1-4688-b7f5-ea07361b26a9",
  "DISPLAY_ONE" = "beb5483e-36e1-4688-b7f5-eaaaaaaaaaaa",
  "DISPLAY_TWO" = "beb5483e-36e1-4688-b7f5-eaaaaaaaaaab",
  "DISPLAY_MODE" = "beb5483e-36e1-4688-b7f5-eaaaaaaaaaac",
  "CONFIG" = "beb5483e-36e1-4688-b7f5-cccccccccccc",
}

export type BLEDataType = {
  [key in DATA_UUIDS]: string;
};

export const UUIDS_KEY_VALUE = Object.entries(DATA_UUIDS);

type BLESettingsStore = {
  isAvailable: boolean;
  isConnected: boolean;
  connecting: boolean;
  failed: boolean;
  device: BluetoothDevice | null;
  server: BluetoothRemoteGATTServer | null;
  services: BluetoothRemoteGATTService[] | null;
  characteristics: BluetoothRemoteGATTCharacteristic[] | null;
};

export const [BLEStore, setBLEStore] = createStore({
  isAvailable: false,
  isConnected: false,
  connecting: false,
  failed: false,
  device: null,
  server: null,
  services: null,
  characteristics: null,
} as BLESettingsStore);

export const [BLEData, setBLEData] = createStore({
  "beb5483e-36e1-4688-b7f5-ea07361b26a8": "",
  "beb5483e-36e1-4688-b7f5-ea07361b26a9": "",
  "beb5483e-36e1-4688-b7f5-eaaaaaaaaaaa": "",
  "beb5483e-36e1-4688-b7f5-eaaaaaaaaaab": "",
  "beb5483e-36e1-4688-b7f5-eaaaaaaaaaac": "",
  "beb5483e-36e1-4688-b7f5-cccccccccccc": "",
} as BLEDataType);

function handleValueChange(event: Event) {
  const char = event.target as BluetoothRemoteGATTCharacteristic;
  const value = char.value;
  if (!value) return;
  const uuid = char.uuid as DATA_UUIDS;
  if (Object.values(DATA_UUIDS).includes(uuid)) {
    const str = new TextDecoder().decode(value);
    setBLEData(uuid, str);
  }
}

export function initBLEonClient() {
  if (typeof navigator.bluetooth !== "undefined") {
    navigator.bluetooth.getAvailability().then((available) => {
      setBLEStore("isAvailable", available);
    });
    navigator.bluetooth.addEventListener(
      "availabilitychanged",
      (event: any) => {
        setBLEStore("isAvailable", event.target as boolean);
      }
    );
  }
}

function delay(ms: number) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

function resetState() {
  if (BLEStore.isConnected) BLEStore.server?.disconnect();
  BLEStore.characteristics?.forEach((char) => {
    char.removeEventListener("characteristicvaluechanged", handleValueChange);
  });
  setBLEStore("isConnected", false);
  BLEStore.device?.removeEventListener(
    "gattserverdisconnected",
    handleDisconnect
  );
  setBLEStore("device", null);
  setBLEStore("server", null);
  setBLEStore("services", null);
  setBLEStore("characteristics", null);
}

function handleDisconnect() {
  resetState();
}

export async function disconnect() {
  if (!BLEStore.isConnected) return;
  try {
    await BLEStore.server?.disconnect();
    resetState();
  } catch (error) {
    console.log(error);
  }
}

export async function connect() {
  if (!BLEStore.isAvailable || BLEStore.isConnected || BLEStore.connecting)
    return;
  setBLEStore("connecting", true);
  setBLEStore("failed", false);
  let device: BluetoothDevice;
  let server: BluetoothRemoteGATTServer;

  // Web browser popup
  try {
    device = await navigator.bluetooth.requestDevice({
      acceptAllDevices: true,
      optionalServices: [serviceUUID],
    });
    device.addEventListener("gattserverdisconnected", () => {
      handleDisconnect();
    });
    if (!device.gatt) {
      await disconnect();
      return;
    }
    server = await device.gatt.connect();
  } catch (error) {
    console.log(error);
    setBLEStore("failed", true);
    resetState();
    return;
  }

  setBLEStore("isConnected", true);
  setBLEStore("connecting", false);
  setBLEStore("failed", false);

  setBLEStore("device", device);
  setBLEStore("server", server);

  // Read all services and characteristics
  try {
    let services = await server.getPrimaryServices();
    setBLEStore("services", services);

    let characteristics: BluetoothRemoteGATTCharacteristic[] = [];
    for (let i = 0; i < services.length; i++) {
      let chars = await services[i].getCharacteristics();
      await delay(100); // Windows and ESP need time to proces the mesage
      characteristics = [...characteristics, ...chars];
    }

    // Enable notifications and set the handle value change callback
    for (let i = 0; i < characteristics.length; i++) {
      const c = characteristics[i];
      if (c.properties.notify) {
        await c.startNotifications();
      }
      c.addEventListener("characteristicvaluechanged", handleValueChange);

      if (c.properties.read) {
        c.readValue();
      }
    }

    setBLEStore("characteristics", characteristics);
  } catch (error) {
    console.error(error);
    setBLEStore("failed", true);
    resetState();
    return;
  }
}
