object FLegend_11011981: TFLegend_11011981
  Left = 452
  Top = 306
  Margins.Left = 2
  Margins.Top = 2
  Margins.Right = 2
  Margins.Bottom = 2
  BorderStyle = bsDialog
  Caption = 'Legend'
  ClientHeight = 224
  ClientWidth = 240
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  Position = poScreenCenter
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  TextHeight = 13
  object gb1: TGroupBox
    Left = 4
    Top = 3
    Width = 231
    Height = 103
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Unit colors'
    TabOrder = 0
    object Label1: TLabel
      Left = 78
      Top = 18
      Width = 109
      Height = 13
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Standard unit (from KB)'
    end
    object Label2: TLabel
      Left = 78
      Top = 39
      Width = 42
      Height = 13
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'User unit'
    end
    object Label3: TLabel
      Left = 78
      Top = 60
      Width = 48
      Height = 13
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Trivial unit'
    end
    object Label5: TLabel
      Left = 78
      Top = 82
      Width = 125
      Height = 13
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Unrecognized bytes in unit'
    end
    object lblUnitStd: TLabel
      Left = 7
      Top = 18
      Width = 64
      Height = 16
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      AutoSize = False
      Caption = 'system'
      Color = clWhite
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Fixedsys'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblUnitUser: TLabel
      Left = 7
      Top = 38
      Width = 64
      Height = 16
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      AutoSize = False
      Caption = 'Unit5'
      Color = clWhite
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Fixedsys'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblUnitTrivial: TLabel
      Left = 7
      Top = 59
      Width = 64
      Height = 16
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      AutoSize = False
      Caption = 'Unit25'
      Color = clWhite
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Fixedsys'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblUnitUserUnk: TLabel
      Left = 7
      Top = 80
      Width = 64
      Height = 15
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      AutoSize = False
      Caption = 'dlgPass'
      Color = clWhite
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Fixedsys'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
  end
  object gb2: TGroupBox
    Left = 4
    Top = 110
    Width = 231
    Height = 83
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Unit types'
    TabOrder = 1
    object Label4: TLabel
      Left = 78
      Top = 18
      Width = 128
      Height = 13
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Non trivial Initializaiton proc'
    end
    object Label6: TLabel
      Left = 78
      Top = 39
      Width = 126
      Height = 13
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Non trivial Finalization proc'
    end
    object Label7: TLabel
      Left = 78
      Top = 60
      Width = 43
      Height = 13
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'ask me :)'
    end
    object lblInit: TLabel
      Left = 7
      Top = 18
      Width = 64
      Height = 16
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      AutoSize = False
      Caption = 'I'
      Color = clWhite
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Fixedsys'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblFin: TLabel
      Left = 7
      Top = 38
      Width = 64
      Height = 16
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      AutoSize = False
      Caption = 'F'
      Color = clWhite
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Fixedsys'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblUnk: TLabel
      Left = 7
      Top = 59
      Width = 64
      Height = 16
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      AutoSize = False
      Caption = '?'
      Color = clWhite
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Fixedsys'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
  end
  object btnOK: TButton
    Left = 81
    Top = 197
    Width = 73
    Height = 25
    Cursor = crHandPoint
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 2
  end
end
