import { ExtensionContext, window } from "vscode";

import {
	LanguageClient,
	LanguageClientOptions,
	ServerOptions,
	TransportKind
} from "vscode-languageclient/node";

let client: LanguageClient;

export async function activate(context: ExtensionContext) {

	console.log("[acorn-lsp-client] Activated");

	const serverPath = process.env.PATH_TO_ACORN_LSP_SERVER;	

	if (!serverPath) {
		window.showErrorMessage("Failed to find environment path PATH_TO_ACORN_LSP_SERVER. This is required to communicate with acorn LSP server and should be set to the path of th executable for acorn lsp");
		return;
	}

	const serverOptions: ServerOptions = {
		run: { command: serverPath, transport: TransportKind.stdio },
		debug: { command: serverPath, transport: TransportKind.stdio }
	};

	const clientOptions: LanguageClientOptions = {
	};

	client = new LanguageClient(
		'acorn-lsp-server',
		'acorn LSP server',
		serverOptions,
		clientOptions
	);

	await client.start();

	context.subscriptions.push(client);

}

export function deactivate(): Thenable<void> | undefined {
	console.log("[acorn-lsp-client] Deactivated");

	if (!client) {
		return undefined;
	}
	
	return client.stop();
}
