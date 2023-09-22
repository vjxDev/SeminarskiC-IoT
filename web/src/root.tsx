// @refresh reload
import { Suspense } from "solid-js";
import {
	useLocation,
	A,
	Body,
	ErrorBoundary,
	FileRoutes,
	Head,
	Html,
	Meta,
	Routes,
	Scripts,
	Title,
} from "solid-start";
import "./root.css";

export default function Root() {
	const location = useLocation();
	const active = (path: string) =>
		path == location.pathname
			? "border-white"
			: "border-transparent hover:border-white";
	return (
		<Html lang="en">
			<Head>
				<Title>SolidStart - With TailwindCSS</Title>
				<Meta charset="utf-8" />
				<Meta name="viewport" content="width=device-width, initial-scale=1" />
			</Head>
			<Body>
				<Suspense>
					<ErrorBoundary>
						<nav class="bg-gray-800">
							<ul class="container flex items-center p-3 text-gray-200">
								<li class={`border-b-2 ${active("/")} mx-1.5 sm:mx-6`}>
									<A href="/">Hello</A>
								</li>
								<li class={`border-b-2 ${active("/ble")} mx-1.5 sm:mx-6`}>
									<A href="/ble">BLE commands</A>
								</li>
								<li class={`border-b-2 ${active("/serial")} mx-1.5 sm:mx-6`}>
									<A href="/serial">Serial commands</A>
								</li>
							</ul>
						</nav>
						<Routes>
							<FileRoutes />
						</Routes>
					</ErrorBoundary>
				</Suspense>
				<Scripts />
			</Body>
		</Html>
	);
}
