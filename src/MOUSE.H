class _mouse {
		private:
			union REGS i, o ;
		public:
			int  Button, X, Y;
			int  initmouse(void);
			void showmouseptr(void);
			void hidemouseptr(void);
			void restrictmouseptr(int x1, int y1, int x2, int y2);
			void getmousepos(void);
			void setmousepos(int XPos,int YPos);
	}Mouse;

/*
 *	FUNCTION : initmouse
 *	INPUT :	none
 *	OUTPUT : Return Code of 0x33
 *	DESCRIPTION : initializes mouse
 */
int
_mouse :: initmouse(void) {
	i.x.ax = 0 ;
	int86 ( 0x33, &i, &o ) ;
	return ( o.x.ax ) ;
}


/*
 *	FUNCTION : showmouseptr
 *	INPUT :	none
 *	OUTPUT : none
 *	DESCRIPTION : display's mouse pointer on screen
 */
void
_mouse :: showmouseptr(void) {
	i.x.ax = 1 ;
	int86 ( 0x33, &i, &o ) ;
}


/*
 *	FUNCTION : hidemouseptr
 *	INPUT :	none
 *	OUTPUT : none
 *	DESCRIPTION : hides the mouse pointer but the mouse is still enabled
 */
void
_mouse :: hidemouseptr(void) {
	i.x.ax = 2;
	int86(0x33, &i, &o);
}

/*
 *	FUNCTION : restrictmouseptr
 *	INPUT :	Left, Top, Width, Height
 *	OUTPUT : none
 *	DESCRIPTION : restricts mouse pointer within specified co-od
 */
void
_mouse :: restrictmouseptr(int x1, int y1, int x2, int y2) {
	i.x.ax = 7 ;
	i.x.cx = x1 ;
	i.x.dx = x2 ;
	int86 ( 0x33, &i, &o ) ;
	i.x.ax = 8 ;
	i.x.cx = y1 ;
	i.x.dx = y2 ;
	int86 ( 0x33, &i, &o ) ;
}


/*
 *	FUNCTION : getmousepos
 *	INPUT :	none
 *	OUTPUT : none
 *	DESCRIPTION : returns the current mouse position and button status
 */
void
_mouse :: getmousepos (void) {
	i.x.ax = 3 ;
	int86 ( 0x33, &i, &o ) ;
	Button = o.x.bx ;
	X = o.x.cx ;
	Y = o.x.dx ;
}

/*
 *	FUNCTION : setmousepos
 *	INPUT :	X and Y co-ordinates
 *	OUTPUT : none
 *	DESCRIPTION : Places the mouse at the specified location
 */
void
_mouse :: setmousepos(int XPos, int YPos) {
	i.x.ax = 4 ;

	i.x.cx = XPos;
	i.x.dx = YPos;
	int86 ( 0x33, &i, &o ) ;
}


