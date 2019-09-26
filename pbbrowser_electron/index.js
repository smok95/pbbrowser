'use strict';
const path = require('path');
const {
	app,
	BrowserWindow,
	dialog,
	shell,
	Menu
} = require('electron');
const {
	is
} = require('electron-util');
const unhandled = require('electron-unhandled');
const debug = require('electron-debug');
const contextMenu = require('electron-context-menu');
const log = require('electron-log');
const argv = require('commander');
const pkg = require('./package.json');

unhandled();
debug();
contextMenu();

// Note: Must match `build.appId` in package.json		
app.setAppUserModelId("com.yuri.pbbrowser");

// Uncomment this before publishing your first version.
// It's commented out as it throws an error if there are no published versions.
// if (!is.development) {
// 	const FOUR_HOURS = 1000 * 60 * 60 * 4;
// 	setInterval(() => {
// 		autoUpdater.checkForUpdates();
// 	}, FOUR_HOURS);
//
// 	autoUpdater.checkForUpdates();
// }

// Prevent window from being garbage collected
let mainWindow;

const createMainWindow = async () => {

	const s = is.development ? 'dev' : 'release';

	if (!is.development) {
		log.transports.console.level = false; // release모드인 경우 console에 로그를 출력하지 않는다.
	}

	const defaultUrl = `file://${__dirname}/index.html`;

	// electron dist버전에서 commanderjs parse가 정상적으로 되지 않는 현상 수정
	if (process.defaultApp != true) {
		process.argv.unshift(null)
	  }

	argv
		.version(pkg.version)
		.option('-u, --url <url>', 'webpage address')
		.option('-w, --width <n>', 'window width')
		.option('-h, --height <n>', 'window height')
		.option('-d, --debug', 'debug mode')
		.parse(process.argv);

	log.debug(process.argv);
	//console.log(process.argv);

	const width = argv.width || 800;
	const height = argv.height || 600;

	log.debug(`width=${width}, height=${height}`);

	const win = new BrowserWindow({
		title: app.getName(),
		show: false,
		width: Number(width),
		height: Number(height),
		useContentSize: true,
		webPreferences: {
			webviewTag: true
		}
	});

	// UserAgent값에 PBBrowser 추가, 19.09.10 kim,jk
	// 이미추가되어 있어 추가안함. win.webContents.setUserAgent(`${win.webContents.getUserAgent()} PBBrowser`);

	if(argv.debug)
	win.webContents.openDevTools();

	win.on('ready-to-show', () => {
		log.debug('ready-to-show');
		win.show();
	});

	win.on('closed', () => {
		// Dereference the window
		// For multiple windows store them in an array
		mainWindow = undefined;
	});

	win.on('unresponsive', () => {
		log.debug('unresponsive');
	});

	win.webContents.session.on('will-download', (event, item, webContents) => {
		item.once('done', (event, state) => {
			if (state === 'completed') {
				log.debug('Download successfully', item.getFilename(), item.getSavePath());
				shell.openItem(item.getSavePath());
			} else {
				log.debug(`Download failed: ${state}`);
			}
		});
	});

	win.webContents.on('did-finish-load', () => {
		log.debug('did-finish-load');
	});
	
	win.webContents.on('did-fail-load', (event, eCode, eDesc, validatedURL, isMainFrame, frameProcessId, frameRoutingId) => {
		//log.debug('did-fail-load', event, eCode, eDesc, validatedURL, isMainFrame, frameProcessId, frameRoutingId);
		if (eCode === -6 || eCode === -105) { // ERR_FILE_NOT_FOUND or ERR_NAME_NOT_RESOLVED
			win.webContents.loadURL(defaultUrl);
		} else {
			console.error(`did-faid-load, ${eCode}:${eDesc}, ${validatedURL}`);
		}
	});

	win.webContents.on('new-window', (event, url, frameName, disposition, options) => {
		const szClient = win.getContentSize();
		const szWindow = win.getSize();

		options.useContentSize = true;

		log.debug('[win.webContents.session]--------------------------------------------');
		log.debug(win.webContents.session);
		log.debug('--------------------------------------------');
		log.debug(event);
		log.debug('--------------------------------------------');
		log.debug(url);
		log.debug('--------------------------------------------');
		log.debug(frameName);
		log.debug('--------------------------------------------');
		log.debug(disposition);
		log.debug('--------------------------------------------');
		log.debug(options);
		log.debug('--------------------------------------------');

		const frameW = szWindow[0] - szClient[0];
		const frameH = szWindow[1] - szClient[1];

		//options.height += frameH;
		//options.width += frameW;

	});

	let url = argv.url;
	if (!url) {
		url = defaultUrl;
	}

	log.debug(argv);

	try {
		//await win.loadFile('index.html');
		await win.loadURL(url);
	} catch (error) {
		console.error(error);
		app.exit(error.errorno);
	}
	//await win.loadFile(path.join(__dirname, 'index.html'));

	log.debug('after loadURL');
	return win;
};

// Prevent multiple instances of the app
if (!app.requestSingleInstanceLock()) {
	app.quit();
}

app.on('second-instance', () => {
	if (mainWindow) {
		if (mainWindow.isMinimized()) {
			mainWindow.restore();
		}

		mainWindow.show();
	}
});

app.on('window-all-closed', () => {
	if (!is.macos) {
		app.quit();
	}
});

app.on('activate', async () => {
	if (!mainWindow) {
		mainWindow = await createMainWindow();
	}
});

app.on('browser-window-focus', (event, window) => {
	console.log('browser-window-focus call');
	console.log(window.getTitle());
	console.log(`isFucused=${window.webContents.isFocused()}`);
});

app.on('browser-window-blur', (event, window) => {
	event.preventDefault();
	console.log('browser-window-blur call');
});

(async () => {
	await app.whenReady();
	Menu.setApplicationMenu(null);
	mainWindow = await createMainWindow();
})();
