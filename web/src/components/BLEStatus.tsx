import { Component, createSignal, For, onMount, Show } from "solid-js";
import {
  connect,
  disconnect,
  BLEStore,
  BLEData,
  DATA_UUIDS,
  UUIDS_KEY_VALUE,
} from "~/store/BLEStore";

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
      await char.writeValue(
        new TextEncoder().encode("Hello World" + char.uuid.at(-1))
      );
  }
}

const RowInTable: Component<{
  name: string;
  value: string;
  uuid: DATA_UUIDS;
}> = (props) => {
  const [edit, setEdit] = createSignal<boolean>(false);
  const [text, setText] = createSignal<string>("");

  function submit() {
    const char = BLEStore.characteristics?.find((char) => {
      return char.uuid === props.uuid;
    });

    char?.writeValue(new TextEncoder().encode(text()));

    setEdit(false);
  }
  return (
    <tr>
      <td class="border px-4 py-2">{props.name}</td>
      <Show when={edit() === false}>
        <td class="border px-4 py-2">{props.value}</td>
        <td class="border ">
          <button
            class="w-full h-full"
            onClick={() => {
              setEdit(true);
            }}
          >
            Edit
          </button>
        </td>
      </Show>
      <Show when={edit() === true}>
        <td class="border px-4 py-2">
          <input
            type="text"
            name="text"
            onInput={(e) => {
              setText(e.target.value);
            }}
          />
        </td>
        <td class="border ">
          <button
            class="w-full h-full"
            onClick={() => {
              submit();
            }}
          >
            Submit
          </button>
        </td>
      </Show>
    </tr>
  );
};

export const Status: Component = () => {
  return (
    <>
      <Show
        when={BLEStore.isAvailable}
        fallback={
          <>
            <p class="text-red-500">Bluetooth is not available</p>
          </>
        }
      >
        <p class="text-green-500">Bluetooth is available</p>
        <Show
          when={BLEStore.isConnected}
          fallback={
            <>
              <button
                class="rounded-lg border border-gray-300 p-4"
                onClick={() => {
                  connect();
                }}
              >
                {" "}
                Select a device
              </button>
            </>
          }
        >
          <p class="text-green-500">Bluetooth is connected</p>
          <button
            class="rounded-lg border border-gray-300 p-4"
            onClick={() => {
              disconnect();
            }}
          >
            {" "}
            Disconnect
          </button>
          <button
            class="rounded-lg border border-gray-300 p-4"
            onClick={() => {
              readAll();
            }}
          >
            {" "}
            Read All
          </button>
          <button
            class="rounded-lg border border-gray-300 p-4"
            onClick={() => {
              writeAll();
            }}
          >
            {" "}
            Write All
          </button>
          {/* create a table with 2 colums */}
          {/* 1st column is the name of the characteristic */}
          {/* 2nd column is the value of the characteristic */}
          {/* if the characteristic is writable, add a button to write to it */}
          <table class="table-auto">
            <thead>
              <tr>
                <th class="px-4 py-2">Name</th>
                <th class="px-4 py-2">Value</th>
                <th class="px-4 py-2">Action</th>
              </tr>
            </thead>
            <tbody>
              <For each={UUIDS_KEY_VALUE}>
                {([name, uuid]) => (
                  <RowInTable value={BLEData[uuid]} name={name} uuid={uuid} />
                )}
              </For>
            </tbody>
          </table>
        </Show>
      </Show>
    </>
  );
};
