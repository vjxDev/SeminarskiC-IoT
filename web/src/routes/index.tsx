import { Status } from "~/components/BLEStatus";

export default function Home() {
  return (
    <main class="grid mx-auto p-8 max-w-xl border  rounded-lg mt-4">
      <h1 class="text-2xl font-bold">Seminarski rad C </h1>
      <p class="text-gray-600">
        {" "}
        <b>by:</b>{" "}
        <a href="https://jovica.me" class="underline text-blue-500">
          Jovica Veljkovic
        </a>
      </p>
      <div
        style="position: relative; width: 100%; height: 0; padding-top: 56.2500%;
 padding-bottom: 0; box-shadow: 0 2px 8px 0 rgba(63,69,81,0.16); margin-top: 1.6em; margin-bottom: 0.9em; overflow: hidden;
 border-radius: 8px; will-change: transform;"
      >
        <iframe
          loading="lazy"
          style="position: absolute; width: 100%; height: 100%; top: 0; left: 0; border: none; padding: 0;margin: 0;"
          src="https:&#x2F;&#x2F;www.canva.com&#x2F;design&#x2F;DAFuPNz-048&#x2F;view?embed"
          allowfullscreen={true}
          allow="fullscreen"
        ></iframe>
      </div>
    </main>
  );
}
