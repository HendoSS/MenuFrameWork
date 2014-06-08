class Slider
{
public:
	/*TODO add templated member function, with some sort of callback for user defined
	modification of templated member (because we won't know how to modify it as the type is unknown*/
	bool IsMouseInBounds(XMFLOAT2 MousePos);
	void Render(DXOverlay* appinst);
	void HandleMouseDown();
	void HandleMouseUP();
};