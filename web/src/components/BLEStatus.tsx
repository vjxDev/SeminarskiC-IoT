import { Component, createSignal, Show } from "solid-js";
import { connect, disconnect, BLEStore, BLEData } from "~/store/BLEStore";

async function readAll() {
	if (!BLEStore.characteristics) return;
	for (const char of BLEStore.characteristics) {
		const val = await char.readValue();
		console.log(val);
	}
}
async function writeAll() {
	if (!BLEStore.characteristics) return;
	for (const char of BLEStore.characteristics) {
		if (char.properties.write)
			await char.writeValue(new TextEncoder().encode("Hello World"));
	}
}

export const Status: Component = () => {

	return (
		<>
			<Show when={BLEStore.isAvailable} fallback={<>
				<p class="text-red-500">Bluetooth is not available</p>
			</>}>
				<p class="text-green-500">Bluetooth is available</p>
				<Show when={BLEStore.isConnected} fallback={<><button onClick={() => { connect() }}> Do you want to connect?</button></>}>
					<p class="text-green-500">Bluetooth is connected</p>
					<button class="rounded-lg border border-gray-300 p-4" onClick={() => { disconnect() }}> Disconnect</button>
				</Show>

			</Show>
			<Show when={BLEData.wifiSSID} fallback={<></>}>
				{BLEData.wifiSSID}
			</Show>
			<button class="rounded-lg border border-gray-300 p-4" onClick={() => { readAll() }}> Read All</button>

			<button class="rounded-lg border border-gray-300 p-4" onClick={() => { writeAll() }}> Write All</button>

		</>
	);
};