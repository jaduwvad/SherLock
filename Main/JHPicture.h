#pragma once


// JHPicture

class JHPicture : public CStatic
{
	DECLARE_DYNAMIC(JHPicture)

public:
	JHPicture();
	virtual ~JHPicture();
	COLORREF transparent;

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
};


