#include <iostream>
#include <string>
#include <windows.h>
#include <CommDlg.h>
#include "resource.h"
#include "Commctrl.h"

using namespace std;

struct Usuario
{
	int id_usuario;
	string clave;
	string contrasena;

	Usuario(int id, string clave, string contrasena) { //constructor
		id_usuario = id;
		this->clave = clave;
		this->contrasena = contrasena;
	}
};

struct Evento
{
	int id_evento;
	string nombre;
	string descripcion;
	string fecha;
	string hora;
	string lugar;
	float precio;
	int capacidad; //cuantos tickets estan disponibles

	Evento()
	{
		id_evento = 0;
		nombre = "";
		descripcion = "";
		fecha = "";
		hora = "";
		lugar = "";
		precio = 0;
		capacidad = 0;
	};

	Evento(int id_evento, string nombre, string descripcion, string fecha, string hora, string lugar, float precio, int capacidad) // se llenan de los datos mandados
	{
		this->id_evento = id_evento;
		this->nombre = nombre;
		this->descripcion = descripcion;
		this->fecha = fecha;
		this->lugar = lugar;
		this->hora = hora;
		this->precio = precio;
		this->capacidad = capacidad;
	}
};

struct Boletos
{
	int id_boleto;
	Evento evento; // se referencia a que boleto va el evento -> se enlaza
	float monto;

	Boletos() 
	{
		id_boleto = 0;
		monto = 0;
	}

	Boletos(int id, Evento evento, float precioFinal) 
	{
		this->id_boleto = id;
		this->evento = evento;
		this->monto = precioFinal;
	}
};

Usuario usuarioRegistrado = Usuario(1, "1000001", "1234"); //Usuario preestablecido

Evento evento1 = Evento(1, "Un Cuento de Navidad. EL MUSICAL", "Disfruta de la conmovedora historia para toda la familia", "26/11/2023", "09:00:00", "Auditorio Pabellón M", 350.50, 100); // ORDENAR
Evento evento2 = Evento(2, "La Casetera", "Regresan a casa el grupo que recopila las mejores canciones que nos han hecho bailar y cantar durante años.", "26/11/2023", "17:00:00", "Alameda Mariano Escobedo", 100.00, 200);
Evento evento3 = Evento(3, "El Show de Bely y Beto", "Bely y Beto regresan a los escenarios", "27/11/2023", "13:00:00", "Sendero Gral. Escobedo", 20.0, 500);
Evento evento4 = Evento(4, "Enanitos Verdes", "¡Asegura tus boletos y prepárate para disfrutar de un gran concierto!", "27/11/2023", "15:00:00", "UANL", 250.50, 90);
Evento evento5 = Evento(5, "90´s Pop Tour", "La máquina del tiempo está de regreso para ofrecer la experiencia del 90’s Pop Tour a otro nivel", "27/11/2023", "19:00:00", "Auditorio GNP Seguros", 630.50, 30);
Evento evento6 = Evento(6, "La fábrica de Santa", "Una mágica historia navideña", "27/11/2023", "21:00:00", "Macroplaza", 550.50, 500);

Evento eventos[6] = { evento1, evento2, evento3, evento4, evento5, evento6 };
Evento eventoSeleccionado;

Boletos compras[20];
Boletos boletoSeleccionado;
int contadorCompras = 0;

HWND hLogInWnd;
HWND hCompraWnd;
HWND hMisComprasWnd;
HWND hAgendaWnd;

void MenuOpcion(HWND hwnd, WPARAM wparam, HWND ventanaCerrar);

LRESULT CALLBACK PROCLogIn(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); //declaracion de la primer ventana
LRESULT CALLBACK PROCEventos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PROCCompra(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PROCMisCompras(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK PROCLogIn(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SHOWWINDOW:
	{
		SetDlgItemText(hwnd, IDC_EDIT_LogIn_ClaveUsu, "1000001");
		SetDlgItemText(hwnd, IDC_EDIT_LogIn_Contrasena, "1234");
	}	break;

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDOK)
		{
			//Se obtiene la clave y contraseña ingresada en el edit
			char* usuario = new char[20];
			char* contra = new char[20];
			GetWindowText(GetDlgItem(hwnd, IDC_EDIT_LogIn_ClaveUsu), usuario, 20);
			GetWindowText(GetDlgItem(hwnd, IDC_EDIT_LogIn_Contrasena), contra, 20);
							
			// Logica para comprobar credenciales
			if (usuarioRegistrado.clave == usuario && usuarioRegistrado.contrasena == contra)
			{
				ShowWindow(hLogInWnd, 0);
				ShowWindow(hAgendaWnd, 1);
			}
			else
			{
				MessageBox(hwnd, "Datos inválidos", "A D V E R T E N C I A", MB_ICONEXCLAMATION  | MB_OK);
			}
		}
	}break;

	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		PostQuitMessage(0);
	}break;


	}
	return 0;
}



LRESULT CALLBACK PROCEventos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SHOWWINDOW:
		SendMessage(GetDlgItem(hwnd, IDC_LISTBOX_Eventos), LB_RESETCONTENT, 0, 0); //elimina los valores del cuadro de text
		for (int i = 0; i < 6; i++)
		{
			string cadena;
			cadena.append(eventos[i].nombre);
			cadena.append(" | ");
			cadena.append(eventos[i].fecha);
			cadena.append(" | ");
			cadena.append(eventos[i].hora);
			
			const char* valor = cadena.c_str();
			SendMessage(GetDlgItem(hwnd, IDC_LISTBOX_Eventos), LB_ADDSTRING, 0, (LPARAM)valor);
		}
		break;

	case WM_COMMAND:
		MenuOpcion(hwnd, wParam, hAgendaWnd);
		if (LOWORD(wParam) == IDC_LISTBOX_Eventos && HIWORD(wParam) == LBN_SELCHANGE)
		{
			int indiceEvento = SendDlgItemMessage(hwnd, IDC_LISTBOX_Eventos, LB_GETCURSEL, 0, 0);
			eventoSeleccionado = eventos[indiceEvento];

			// Mostrar datos
			SetDlgItemText(hwnd, IDC_STATIC_Eventos_Fecha, (LPCSTR)eventos[indiceEvento].fecha.c_str());
			SetDlgItemText(hwnd, IDC_STATIC_Eventos_Hora, (LPCSTR)eventos[indiceEvento].hora.c_str());
			SetDlgItemText(hwnd, IDC_STATIC_Eventos_Obra, (LPCSTR)eventos[indiceEvento].nombre.c_str());
			SetDlgItemText(hwnd, IDC_STATIC_Eventos_Locacion, (LPCSTR)eventos[indiceEvento].lugar.c_str());
		}

		if (LOWORD(wParam) == IDC_BUTTON_Eventos_Comprar) 
		{
			int indice = SendDlgItemMessage(hwnd, IDC_LISTBOX_Eventos, LB_GETCURSEL, 0, 0);
			if (indice == -1)
			{
				MessageBox(hwnd, "No se ha seleccionado un evento", " A D V E R T E N C I A", MB_ICONERROR | MB_OK);
				break;
			}
			eventoSeleccionado = eventos[indice];
			ShowWindow(hAgendaWnd, 0);
			ShowWindow(hCompraWnd, 1);
		}
		break;

	case WM_CLOSE:
	{
		int MB = MessageBox(hwnd, "¿Desea salir?", "A D V E R T E N C I A ", MB_ICONEXCLAMATION | MB_OKCANCEL);
		if (MB == IDOK)
		{
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}
	}break;

	}
	return 0;
}



LRESULT CALLBACK PROCCompra(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	switch(msg) 
	{
	case WM_SHOWWINDOW:
	{
		SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_RESETCONTENT, 0, 0);
		SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM(" | Sin promoción"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM(" | Promoción 2 x 1"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM(" | Promoción 10%" ));
		SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_SETCURSEL, 0, 0);
		
		SetDlgItemText(hwnd, IDC_STATIC_Compra_NombreObra, (LPCSTR)eventoSeleccionado.nombre.c_str());
		SetDlgItemText(hwnd, IDC_STATIC_Compra_Descripcion, (LPCSTR)eventoSeleccionado.descripcion.c_str());
		SetDlgItemText(hwnd, IDC_STATIC_Compras_Lugar, (LPCSTR)eventoSeleccionado.lugar.c_str());
		SetDlgItemText(hwnd, IDC_STATIC_Compra_FECHA, (LPCSTR)eventoSeleccionado.fecha.c_str());
		SetDlgItemText(hwnd, IDC_STATIC_Compra_Hora, (LPCSTR)eventoSeleccionado.hora.c_str());

		// Calcular precio final
		float precioFinal = eventoSeleccionado.precio * 1.16;
		SetDlgItemText(hwnd, IDC_STATIC_Compra_PrecioFinal, (LPCSTR)to_string(precioFinal).c_str());
		
	} break;

	case WM_COMMAND:
		MenuOpcion(hwnd, wParam, hCompraWnd);
		if (LOWORD(wParam) == IDC_COMBO1 && HIWORD(wParam) == CBN_CLOSEUP)
		{
			// Recalcular precio final
			// Aplicar promo / descuento
			int indicePromo = SendDlgItemMessage(hwnd, IDC_COMBO1, CB_GETCURSEL, 0, 0);
			float precioFinal = eventoSeleccionado.precio;
			switch (indicePromo) {
			case 0: case 1:
				precioFinal = precioFinal * 1.16;
				break;
			case 2:
				precioFinal = precioFinal * 1.16 * 0.9;
				break;
			}
			SetDlgItemText(hwnd, IDC_STATIC_Compra_PrecioFinal, (LPCSTR)to_string(precioFinal).c_str());
		}

		if (LOWORD(wParam) == IDOK_Confirmar)
		{
			char* buffer = new char[256];
			GetDlgItemText(hwnd, IDC_STATIC_Compra_PrecioFinal, buffer, 256);
			float precioFinal = atof(buffer);
			compras[contadorCompras] = Boletos(contadorCompras, eventoSeleccionado, precioFinal);
			
			contadorCompras++;
			ShowWindow(hCompraWnd, 0);
			ShowWindow(hAgendaWnd, 1);
			MessageBox(hwnd, "La compra se ha realizado correctamente", "T A Q U I L L A", MB_ICONINFORMATION | MB_OK);
		}

		if (LOWORD(wParam) == IDCANCEL_Compra)
		{
			ShowWindow(hCompraWnd, 0);
			ShowWindow(hAgendaWnd, 1);
			MessageBox(hwnd, "La compra se ha cancelado", "A D V E R T E N C I A", MB_ICONWARNING | MB_OK);
		}
		break;

	case WM_CLOSE:
	{
		int MB = MessageBox(hwnd, "¿Desea salir?", "A D V E R T E N C I A ", MB_ICONEXCLAMATION | MB_OKCANCEL);
		if (MB == IDOK)
		{
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}
	}break;
	}
	return 0;
}



LRESULT CALLBACK PROCMisCompras(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SHOWWINDOW:
		SendMessage(GetDlgItem(hwnd, IDC_LISTBOX_Boletos), LB_RESETCONTENT, 0, 0); //elimina los valores del cuadro de text
		for (int i = 0; i < contadorCompras; i++)
		{
			string cadena;
			cadena.append(compras[i].evento.nombre);
			cadena.append(" | ");
			cadena.append(compras[i].evento.fecha);
			cadena.append(" | ");
			cadena.append(compras[i].evento.hora);
			cadena.append(" | ");


			const char* valor = cadena.c_str();
			SendMessage(GetDlgItem(hwnd, IDC_LISTBOX_Boletos), LB_ADDSTRING, 0, (LPARAM)valor);
		}
		break;

		break;
	case WM_COMMAND:
	{
		MenuOpcion(hwnd, wParam, hMisComprasWnd);
		if (LOWORD(wParam) == IDC_LISTBOX_Boletos && HIWORD(wParam) == LBN_SELCHANGE)
		{
			int indiceBoleto = SendDlgItemMessage(hwnd, IDC_LISTBOX_Boletos, LB_GETCURSEL, 0, 0);
			boletoSeleccionado = compras[indiceBoleto];

			// Mostrar datos
			SetDlgItemText(hwnd, IDC_STATIC_MisCompras_Fecha, (LPCSTR)boletoSeleccionado.evento.fecha.c_str());
			SetDlgItemText(hwnd, IDC_STATIC_MisCompras_Hora, (LPCSTR)boletoSeleccionado.evento.hora.c_str());
			SetDlgItemText(hwnd, IDC_STATIC_MisCompras_Nombre, (LPCSTR)boletoSeleccionado.evento.nombre.c_str());
			SetDlgItemText(hwnd, IDC_STATIC_MisCompras_Lugar, (LPCSTR)boletoSeleccionado.evento.lugar.c_str());
		}

	}
	break;

	case WM_CLOSE:
	{
		int MB = MessageBox(hwnd, "¿Desea salir?", "A D V E R T E N C I A ", MB_ICONEXCLAMATION | MB_OKCANCEL);
		if (MB == IDOK)
		{
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}
	}break;

	}
	return 0;
}

void MenuOpcion(HWND hwnd, WPARAM wParam, HWND ventanaCerrar)
{
	if (LOWORD(wParam) == ID_EVENTOS)
	{
		ShowWindow(ventanaCerrar, 0);
		ShowWindow(hAgendaWnd, 1);
	}
	if (LOWORD(wParam) == ID_MISCOMPRAS)
	{
		ShowWindow(ventanaCerrar, 0);
		ShowWindow(hMisComprasWnd, 1);
	}
	if (LOWORD(wParam) == ID_SALIR)
	{
		DestroyWindow(hwnd);
		PostQuitMessage(0);
	}
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevINstance, LPSTR lpszCmdParam, int nCmdShow)
{

	hLogInWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_LogIn), NULL, PROCLogIn);
	hAgendaWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_EVENTOS), NULL, PROCEventos);
	hCompraWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_COMPRA), NULL, PROCCompra);
	hMisComprasWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MisCompras), NULL, PROCMisCompras);

	ShowWindow(hLogInWnd, nCmdShow);

	/* Bucle de mensajes: | ESPERA QUE HAY QUE HACER: sin este la ventana no "aparece" */
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}