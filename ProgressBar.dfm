object FProgressBar: TFProgressBar
  Left = 449
  Top = 484
  BorderIcons = []
  BorderStyle = bsToolWindow
  ClientHeight = 43
  ClientWidth = 580
  Color = clBtnFace
  DefaultMonitor = dmMainForm
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -10
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  Position = poScreenCenter
  OnShow = FormShow
  TextHeight = 13
  object pb: TProgressBar
    Left = 0
    Top = 0
    Width = 580
    Height = 24
    Align = alClient
    BorderWidth = 4
    Max = 256
    Smooth = True
    TabOrder = 0
    ExplicitWidth = 480
  end
  object sb: TStatusBar
    Left = 0
    Top = 24
    Width = 580
    Height = 19
    Panels = <
      item
        Text = '000'
        Width = 280
      end
      item
        Text = '111'
        Width = 300
      end>
    ExplicitWidth = 480
  end
end
