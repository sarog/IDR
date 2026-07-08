object FHex2DoubleDlg_11011981: TFHex2DoubleDlg_11011981
  Left = 384
  Top = 341
  Margins.Left = 2
  Margins.Top = 2
  Margins.Right = 2
  Margins.Bottom = 2
  BorderStyle = bsDialog
  Caption = 'Hex->Double Converter'
  ClientHeight = 76
  ClientWidth = 643
  Color = clBtnFace
  ParentFont = True
  Position = poScreenCenter
  OnCreate = FormCreate
  OnShow = FormShow
  TextHeight = 15
  object rgDataViewStyle: TRadioGroup
    Left = 0
    Top = 0
    Width = 643
    Height = 39
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Align = alTop
    Columns = 6
    ItemIndex = 0
    Items.Strings = (
      'Hex'
      'Single (32bit)'
      'Double (64)'
      'Extended (80)'
      'Real (32)'
      'Comp (64)')
    TabOrder = 0
    OnClick = rgDataViewStyleClick
  end
  object edtValue: TLabeledEdit
    Left = 64
    Top = 45
    Width = 558
    Height = 21
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    EditLabel.Width = 36
    EditLabel.Height = 21
    EditLabel.Caption = 'Value:'
    EditLabel.Font.Charset = RUSSIAN_CHARSET
    EditLabel.Font.Color = clWindowText
    EditLabel.Font.Height = -13
    EditLabel.Font.Name = 'Fixedsys'
    EditLabel.Font.Style = []
    EditLabel.ParentFont = False
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Fixedsys'
    Font.Style = []
    LabelPosition = lpLeft
    ParentFont = False
    TabOrder = 1
    Text = ''
    OnEnter = edtValueEnter
  end
end
