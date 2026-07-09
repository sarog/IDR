object FExit_11011981: TFExit_11011981
  Left = 482
  Top = 333
  Margins.Left = 2
  Margins.Top = 2
  Margins.Right = 2
  Margins.Bottom = 2
  BorderStyle = bsDialog
  Caption = 'Save Project'
  ClientHeight = 125
  ClientWidth = 306
  Color = clBtnFace
  ParentFont = True
  Position = poScreenCenter
  OnCreate = FormCreate
  TextHeight = 15
  object Bevel1: TBevel
    Left = 8
    Top = 8
    Width = 292
    Height = 80
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Shape = bsFrame
  end
  object OKBtn: TButton
    Left = 62
    Top = 98
    Width = 74
    Height = 25
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = OKBtnClick
  end
  object CancelBtn: TButton
    Left = 172
    Top = 98
    Width = 74
    Height = 25
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
    OnClick = CancelBtnClick
  end
  object cbDontSaveProject: TCheckBox
    Left = 51
    Top = 39
    Width = 206
    Height = 17
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Don'#39't save Project'
    TabOrder = 2
  end
end
