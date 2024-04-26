#include <Windows.h>

// Прототип функции обработки сообщений с пользовательским названием
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR mainMessage[] = L"Какой-то текст!";

int WINAPI WinMain(	HINSTANCE hInst,		// Дескриптор экзепляра приложения
					HINSTANCE hPrevInst,    // 
					LPSTR lpCmdLine,		//
					int nCmdShow			// Режим отображения окна
) {
	TCHAR szClassName[] = L"Мой класс";		// Строка с именем класса
	HWND hMainWnd{};						// Создаём дескриптор будущего окна
	MSG msg{};								// Создаём экземпляр структуры, для обработки сообщений
	WNDCLASSEX wc{};						// Экземпляр, для обращения к членам класса WNDCLASSEX

	wc.cbSize			= sizeof(wc);							// Размер структуры в байтах
	wc.style			= CS_HREDRAW | CS_VREDRAW;				// Стиль класса окна
	wc.lpfnWndProc		= WndProc;								// Указатель на пользовательскую функцию
	wc.lpszMenuName		= NULL;									// Указатель на имя меню (тут его нет)
	wc.lpszClassName	= szClassName;							// Указатель на имя класса
	wc.cbWndExtra		= NULL;									// Число освобождаемых байтов в конце структуры
	wc.cbClsExtra		= NULL;									// Число освобождаемых байтов при создании экземпляра приложения
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Дескриптор пиктограммы
	wc.hIconSm			= LoadIcon(NULL, IDI_WINLOGO);			// Дескриптор маленькой пиктограммы в трэе
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Дескриптор курсора
	wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);	// Дескриптор кисти для закраски фона
	wc.hInstance		= hInst;								// Указатель на строку, содержащую имя меню, применяемого для класса

	if (!RegisterClassEx(&wc)) {
		// В случае отсутствия регистрации класса
		MessageBox(NULL, L"Не получилось зарегистрировать класс", L"Ошибка", MB_OK);

		return NULL; // Выход из WinMain
	}

	hMainWnd = CreateWindow( szClassName,						// Имя класса
							 L"Полнощенная оконная процедура",	// Имя окна 
							 WS_OVERLAPPEDWINDOW | WS_VSCROLL,	// Режим отображения окна
							 CW_USEDEFAULT,						// Позиция окна по оси х
							 NULL,								// Позиция окна по оси у (Если дефолт в х то писать не надо)
							 CW_USEDEFAULT,						// Ширина окна
							 NULL,								// Высота окна
							 (HWND)NULL,						// Дескриптор род окна
							 NULL,								// Дескриптор меню
							 HINSTANCE(hInst),					// Дескриптор экземпляра меню
							 NULL);								//

	if (!hMainWnd) {
		// В случае некорректного создания окна
		MessageBox(NULL, L"Не получилось создать окно", L"Ошибка!", MB_OK);

		return NULL;
	}
	ShowWindow(hMainWnd, nCmdShow);		// Отображение окна
	UpdateWindow(hMainWnd);				// Обновление окна

	// Ивзлекаем сообщения из очереди, посылаемы ф-циями ОС
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);			// Интерпретация сообщения
		DispatchMessage(&msg);			// Передача сообщения обратно ОС
	}

	return msg.wParam;					// Выход из приложения
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HDC hDC;								// Дескриптор ориентации текста 
	PAINTSTRUCT ps;							// Структура содержит информацию о клиентской обл (цвет, размер и тп)
	RECT rect;								// Аналогичная структура
	COLORREF colorText = RGB(255, 0, 0);    // Цвет текста 

	switch (uMsg) {
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);													// Инициализируем контекст устройства
		GetClientRect(hWnd, &rect);														// Получаем ширину и высоту области для рисования
		SetTextColor(hDC, colorText);													// Устанавливаем цвет контекстного устройства
		DrawText(hDC, mainMessage, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);	// Рисуем текст
		EndPaint(hWnd, &ps);															// Заканчиваем рисовать

		break;
	case WM_DESTROY:			// Если окно закрыто
		PostQuitMessage(NULL);	// Отправляем WinMain() сообщение WM_QUIT

		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);	// Если закрыли окно
	}

	return NULL;
}