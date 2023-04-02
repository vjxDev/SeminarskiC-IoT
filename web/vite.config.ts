import solid from "solid-start/vite";
import { defineConfig } from "vite";
import stat from "solid-start-static";

export default defineConfig({
	plugins: [solid({ adapter: stat() })],
});
