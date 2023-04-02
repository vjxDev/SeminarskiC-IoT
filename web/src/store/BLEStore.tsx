
import { createStore } from "solid-js/store";


const wifiServiceUUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
const wifiSSIDUUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

type BLESettingsStore = {
	isAvailable: boolean;
	isConnected: boolean;
	connecting: boolean;
	failed: boolean;
	device: BluetoothDevice | null;
	server: BluetoothRemoteGATTServer | null;
	services: BluetoothRemoteGATTService[] | null;
	characteristics: BluetoothRemoteGATTCharacteristic[] | null;
}

type BLEData = {
	wifiSSID: string;
}

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

export const [BLEData, setBLEData] = createStore({} as BLEData);

export function initBLEonClient() {
	if (typeof navigator.bluetooth !== "undefined") {
		navigator.bluetooth.getAvailability().then((available) => { setBLEStore("isAvailable", available); });
		navigator.bluetooth.addEventListener("availabilitychanged", (event: any) => { setBLEStore("isAvailable", event.target as boolean); });
	}
}

function delay(ms: number) {
	return new Promise(resolve => setTimeout(resolve, ms));
}

function handleValueChange(event: Event) {
	const char = event.target as BluetoothRemoteGATTCharacteristic;
	const value = char.value;
	if (!value) return;
	console.log(value);


	switch (char.uuid) {
		case wifiSSIDUUID:
			const str = new TextDecoder().decode(value);
			setBLEData("wifiSSID", str);
			break;
		default:
			const hello = new TextDecoder().decode(value);
			console.log(hello);
	}
}
function resetState() {
	if (BLEStore.isConnected) BLEStore.server?.disconnect();
	BLEStore.characteristics?.forEach((char) => {
		char.removeEventListener("characteristicvaluechanged", handleValueChange);
	});
	setBLEStore("isConnected", false);
	BLEStore.device?.removeEventListener("gattserverdisconnected", handleDisconnect);
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

	}
}

export async function connect() {
	if (!BLEStore.isAvailable || BLEStore.isConnected || BLEStore.connecting) return;
	setBLEStore("connecting", true);
	setBLEStore("failed", false);
	let device: BluetoothDevice;
	let server: BluetoothRemoteGATTServer;
	try {
		device = await navigator.bluetooth.requestDevice({
			acceptAllDevices: true,
			optionalServices: [wifiServiceUUID]
		});
		device.addEventListener("gattserverdisconnected", () => { handleDisconnect(); });
		if (!device.gatt) {
			await disconnect();
			return;
		}
		server = await device.gatt.connect();
	} catch (error) {
		console.log(error);
		setBLEStore("failed", true);
		resetState()
		return;
	}

	setBLEStore("failed", false);
	setBLEStore("connecting", false);
	setBLEStore("isConnected", true);
	setBLEStore("device", device);
	setBLEStore("server", server);



	try {

		let services = await server.getPrimaryServices();
		setBLEStore("services", services);

		let characteristics: BluetoothRemoteGATTCharacteristic[] = [];
		for (let i = 0; i < services.length; i++) {
			let chars = await services[i].getCharacteristics();
			await delay(100);
			characteristics = [...characteristics, ...chars];
		}

		for (let i = 0; i < characteristics.length; i++) {
			const c = characteristics[i];
			if (c.properties.notify) {
				await c.startNotifications();
			}
			c.addEventListener("characteristicvaluechanged", handleValueChange);
		}

		setBLEStore("characteristics", characteristics);

	} catch (error) {
		console.log(error);
		setBLEStore("failed", true);
		resetState()
		return;
	}

}


