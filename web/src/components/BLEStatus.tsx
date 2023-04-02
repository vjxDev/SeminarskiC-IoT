import { Component, createSignal, For, Show } from "solid-js";
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

const RowInTable: Component<{ name: string, value: string }> = (props) => {
	return (
		<Show when={props.value} fallback={<></>}>
			<tr>
				<td class="border px-4 py-2">{props.name}</td>
				<td class="border px-4 py-2">{props.value}</td>
			</tr>
		</Show>
	)
}

export const Status: Component = () => {

	return (
		<>
			<Show when={BLEStore.isAvailable} fallback={<>
				<p class="text-red-500">Bluetooth is not available</p>
			</>}>
				<p class="text-green-500">Bluetooth is available</p>
				<Show when={BLEStore.isConnected} fallback={<><button class="rounded-lg border border-gray-300 p-4" onClick={() => { connect() }}> Select a device</button></>}>
					<p class="text-green-500">Bluetooth is connected</p>
					<button class="rounded-lg border border-gray-300 p-4" onClick={() => { disconnect() }}> Disconnect</button>
					<button class="rounded-lg border border-gray-300 p-4" onClick={() => { readAll() }}> Read All</button>
					<button class="rounded-lg border border-gray-300 p-4" onClick={() => { writeAll() }}> Write All</button>
					{/* create a table with 2 colums */}
					{/* 1st column is the name of the characteristic */}
					{/* 2nd column is the value of the characteristic */}
					{/* if the characteristic is writable, add a button to write to it */}
					<table class="table-auto">
						<thead>
							<tr>
								<th class="px-4 py-2">Name</th>
								<th class="px-4 py-2">Value</th>
							</tr>
						</thead>
						<tbody>
							<RowInTable value={BLEData.wifiSSID} name="Wifi" />
						</tbody>
					</table>
				</Show>
			</Show>
		</>
	);
};