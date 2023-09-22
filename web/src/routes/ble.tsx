import { A } from "solid-start";
import { Status } from "~/components/BLEStatus";

export default function About() {
	return (
		<main class="grid mx-auto p-8 max-w-xl border  rounded-lg mt-4 gap-4">
			<h1 class="text-2xl font-bold">Seminarski rad C </h1>
			<p class="text-gray-600"> <b>by:</b> <a href="https://jovica.me" class="underline text-blue-500">Jovica Veljkovic</a></p>
			<Status />
		</main>
	);
}
