    using System;  
    using System.Collections.Generic;  
    using System.Drawing;  
    using System.Drawing.Drawing2D;  
      
    namespace RotateText  
    {  
        public class GraphicsText  
        {  
            private Graphics _graphics;  
      
            public GraphicsText()  
            {  
      
            }  
      
            public Graphics Graphics  
            {  
                get { return _graphics; }  
                set { _graphics = value; }  
            }  
      
            /// <summary>  
            /// ���Ƹ��ݾ�����ת�ı�  
            /// </summary>  
            /// <param name="s">�ı�</param>  
            /// <param name="font">����</param>  
            /// <param name="brush">���</param>  
            /// <param name="layoutRectangle">�ֲ�����</param>  
            /// <param name="format">���ַ�ʽ</param>  
            /// <param name="angle">�Ƕ�</param>  
            public void DrawString(string s, Font font, Brush brush, RectangleF layoutRectangle, StringFormat format, float angle)  
            {  
                // ��ȡ�ַ�����С  
                SizeF size = _graphics.MeasureString(s, font);  
      
                // ������ת�Ƕȣ���ȡ��ת���ַ�����С  
                SizeF sizeRotate = ConvertSize(size, angle);  
      
                // ������ת��ߴ硢���־��Ρ����ַ�ʽ�����ı���ת��  
                PointF rotatePt = GetRotatePoint(sizeRotate, layoutRectangle, format);  
      
                // ���貼�ַ�ʽ��ΪCenter  
                StringFormat newFormat = new StringFormat(format);  
                newFormat.Alignment = StringAlignment.Center;  
                newFormat.LineAlignment = StringAlignment.Center;  
      
                // ������ת���ı�  
                DrawString(s, font, brush, rotatePt, newFormat, angle);  
            }  
      
            /// <summary>  
            /// ���Ƹ��ݵ���ת�ı���һ����ת�����λ�ı���Χ�����ĵ�  
            /// </summary>  
            /// <param name="s">�ı�</param>  
            /// <param name="font">����</param>  
            /// <param name="brush">���</param>  
            /// <param name="point">��ת��</param>  
            /// <param name="format">���ַ�ʽ</param>  
            /// <param name="angle">�Ƕ�</param>  
            public void DrawString(string s, Font font, Brush brush, PointF point, StringFormat format, float angle)  
            {  
                // Save the matrix  
                Matrix mtxSave = _graphics.Transform;  
      
                Matrix mtxRotate = _graphics.Transform;  
                mtxRotate.RotateAt(angle, point);  
                _graphics.Transform = mtxRotate;  
      
                _graphics.DrawString(s, font, brush, point, format);  
      
                // Reset the matrix  
                _graphics.Transform = mtxSave;  
            }  
      
            private SizeF ConvertSize(SizeF size, float angle)  
            {  
                Matrix matrix = new Matrix();  
                matrix.Rotate(angle);  
      
                // ��ת�����ĸ�����  
                PointF[] pts = new PointF[4];  
                pts[0].X = -size.Width / 2f;  
                pts[0].Y = -size.Height / 2f;  
                pts[1].X = -size.Width / 2f;  
                pts[1].Y = size.Height / 2f;  
                pts[2].X = size.Width / 2f;  
                pts[2].Y = size.Height / 2f;  
                pts[3].X = size.Width / 2f;  
                pts[3].Y = -size.Height / 2f;  
                matrix.TransformPoints(pts);  
      
                // ��ȡ�ĸ�����İ�Χ��  
                float left = float.MaxValue;  
                float right = float.MinValue;  
                float top = float.MaxValue;  
                float bottom = float.MinValue;  
      
                foreach(PointF pt in pts)  
                {  
                    // ��ȡ����  
                    if(pt.X < left)  
                        left = pt.X;  
                    if(pt.X > right)  
                        right = pt.X;  
                    if(pt.Y < top)  
                        top = pt.Y;  
                    if(pt.Y > bottom)  
                        bottom = pt.Y;  
                }  
      
                SizeF result = new SizeF(right - left, bottom - top);  
                return result;  
            }  
      
            private PointF GetRotatePoint(SizeF size, RectangleF layoutRectangle, StringFormat format)  
            {  
                PointF pt = new PointF();  
      
                switch (format.Alignment)  
                {  
                    case StringAlignment.Near:  
                        pt.X = layoutRectangle.Left + size.Width / 2f;  
                        break;  
                    case StringAlignment.Center:  
                        pt.X = (layoutRectangle.Left + layoutRectangle.Right) / 2f;  
                        break;  
                    case StringAlignment.Far:  
                        pt.X = layoutRectangle.Right - size.Width / 2f;  
                        break;  
                    default:  
                        break;  
                }  
      
                switch (format.LineAlignment)  
                {  
                    case StringAlignment.Near:  
                        pt.Y = layoutRectangle.Top + size.Height / 2f;  
                        break;  
                    case StringAlignment.Center:  
                        pt.Y = (layoutRectangle.Top + layoutRectangle.Bottom) / 2f;  
                        break;  
                    case StringAlignment.Far:  
                        pt.Y = layoutRectangle.Bottom - size.Height / 2f;  
                        break;  
                    default:  
                        break;  
                }  
      
                return pt;  
            }  
        }  
    }  


���Դ������£�

[csharp] view plain copy
��CODE�ϲ鿴����Ƭ�������ҵĴ���Ƭ

    using System;  
    using System.Collections.Generic;  
    using System.ComponentModel;  
    using System.Data;  
    using System.Drawing;  
    using System.Windows.Forms;  
      
    namespace RotateText  
    {  
        public partial class FormMain : Form  
        {  
            private Font _font = new Font("Arial", 12);  
            private Brush _brush = new SolidBrush(Color.Black);  
            private Pen _pen = new Pen(Color.Black, 1f);  
            private string _text = "Crow Soft";  
      
            public FormMain()  
            {  
                InitializeComponent();  
            }  
      
            protected override void OnPaint(PaintEventArgs e)  
            {  
                base.OnPaint(e);  
      
                GraphicsText graphicsText = new GraphicsText();  
                graphicsText.Graphics = e.Graphics;  
      
                // ����Χ�Ƶ���ת���ı�  
                StringFormat format = new StringFormat();  
                format.Alignment = StringAlignment.Center;  
                format.LineAlignment = StringAlignment.Center;  
      
                graphicsText.DrawString(_text, _font, _brush, new PointF(100, 80), format, 45f);  
                graphicsText.DrawString(_text, _font, _brush, new PointF(200, 80), format, -45f);  
                graphicsText.DrawString(_text, _font, _brush, new PointF(300, 80), format, 90f);  
                graphicsText.DrawString(_text, _font, _brush, new PointF(400, 80), format, -60f);  
      
                // ���ƾ�������ת���ı�  
                // First line  
                RectangleF rc = RectangleF.FromLTRB(50, 150, 200, 230);  
                RectangleF rect = rc;  
                format.Alignment = StringAlignment.Near;  
      
                e.Graphics.DrawRectangle(_pen, rect.Left, rect.Top, rect.Width, rect.Height);  
                graphicsText.DrawString(_text, _font, _brush, rect, format, 30);  
      
                rect.Location += new SizeF(180, 0);  
                format.LineAlignment = StringAlignment.Near;  
                e.Graphics.DrawRectangle(_pen, rect.Left, rect.Top, rect.Width, rect.Height);  
                graphicsText.DrawString(_text, _font, _brush, rect, format, -30);  
      
                rect.Location += new SizeF(180, 0);  
                format.LineAlignment = StringAlignment.Center;  
                e.Graphics.DrawRectangle(_pen, rect.Left, rect.Top, rect.Width, rect.Height);  
                graphicsText.DrawString(_text, _font, _brush, rect, format, -90);  
      
                rect.Location += new SizeF(180, 0);  
                format.LineAlignment = StringAlignment.Far;  
                e.Graphics.DrawRectangle(_pen, rect.Left, rect.Top, rect.Width, rect.Height);  
                graphicsText.DrawString(_text, _font, _brush, rect, format, 70);  
      
                // Second line  
                rect = rc;  
                rect.Location += new SizeF(0, 100);  
                format.Alignment = StringAlignment.Center;  
      
                e.Graphics.DrawRectangle(_pen, rect.Left, rect.Top, rect.Width, rect.Height);  
                graphicsText.DrawString(_text, _font, _brush, rect, format, 40);  
      
                rect.Location += new SizeF(180, 0);  
                format.LineAlignment = StringAlignment.Near;  
                e.Graphics.DrawRectangle(_pen, rect.Left, rect.Top, rect.Width, rect.Height);  
                graphicsText.DrawString(_text, _font, _brush, rect, format, 30);  
      
                rect.Location += new SizeF(180, 0);  
                format.LineAlignment = StringAlignment.Center;  
                e.Graphics.DrawRectangle(_pen, rect.Left, rect.Top, rect.Width, rect.Height);  
                graphicsText.DrawString(_text, _font, _brush, rect, format, -70);  
      
                rect.Location += new SizeF(180, 0);  
                format.LineAlignment = StringAlignment.Far;  
                e.Graphics.DrawRectangle(_pen, rect.Left, rect.Top, rect.Width, rect.Height);  
                graphicsText.DrawString(_text, _font, _brush, rect, format, 60);  
      
                // Third line  
                rect = rc;  
                rect.Location += new SizeF(0, 200);  
                format.Alignment = StringAlignment.Far;  
      
                e.Graphics.DrawRectangle(_pen, rect.Left, rect.Top, rect.Width, rect.Height);  
                graphicsText.DrawString(_text, _font, _brush, rect, format, -30);  
      
                rect.Location += new SizeF(180, 0);  
                format.LineAlignment = StringAlignment.Near;  
                e.Graphics.DrawRectangle(_pen, rect.Left, rect.Top, rect.Width, rect.Height);  
                graphicsText.DrawString(_text, _font, _brush, rect, format, -30);  
      
                rect.Location += new SizeF(180, 0);  
                format.LineAlignment = StringAlignment.Center;  
                e.Graphics.DrawRectangle(_pen, rect.Left, rect.Top, rect.Width, rect.Height);  
                graphicsText.DrawString(_text, _font, _brush, rect, format, 90);  
      
                rect.Location += new SizeF(180, 0);  
                format.LineAlignment = StringAlignment.Far;  
                e.Graphics.DrawRectangle(_pen, rect.Left, rect.Top, rect.Width, rect.Height);  
                graphicsText.DrawString(_text, _font, _brush, rect, format, 45);  
            }  
        }  
    }  