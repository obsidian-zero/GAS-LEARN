#include "ExtendMenuBase.h"
#include "LevelEditor.h"
#include "Widgets/Layout/SConstraintCanvas.h"

#define LOCTEXT_NAMESPACE "FExtendMenuBaseModule"

void FExtendMenuBaseModule::StartupModule()
{
    IModuleInterface::StartupModule();

    ExtendMenuByFExtendMenuBaseModule();
}

void FExtendMenuBaseModule::ShutdownModule()
{
    IModuleInterface::ShutdownModule();
}

void FExtendMenuBaseModule::ExtendMenuByFExtendMenuBaseModule()
{
    TSharedPtr<FExtender> MainMenuExtender = MakeShareable(new FExtender);
    
    // 起始于此处
    MainMenuExtender->AddMenuExtension(
        "EpicGamesHelp",		// 用于定位的ExtensionHook名称
        EExtensionHook::After,	        // 应添加到的具体位置(该Hook之前还是之后)
        nullptr,			// CommandList我们先暂时缺省
        FMenuExtensionDelegate::CreateRaw(
        this, &FExtendMenuBaseModule::MakeExistingMainMenuEntry)	// 绑定制作按钮的代理函数
        );

    // 起始于此处
    MainMenuExtender->AddMenuBarExtension(
        "Help",
        EExtensionHook::After,
        nullptr,
        FMenuBarExtensionDelegate::CreateRaw(this, &FExtendMenuBaseModule::MakeNewMenuBarEntry)
        );
    
    FLevelEditorModule& LevelEditorModule = 
    FModuleManager::Get().LoadModuleChecked<FLevelEditorModule>("LevelEditor");

    TSharedPtr<FExtensibilityManager> MenuExtensibilityManager = 
    LevelEditorModule.GetMenuExtensibilityManager();

    MenuExtensibilityManager->AddExtender(MainMenuExtender);
}

void FExtendMenuBaseModule::MakeExistingMainMenuEntry(FMenuBuilder& MenuBuilder)
{
    MenuBuilder.AddMenuEntry(
        FText::FromString("Existing Hook Button"),	        // 按钮名称
        FText::FromString("This is a existing hook extend"),	// 按钮提示
        FSlateIcon(),	            // 我们可以为按钮添加图标，这里暂时缺省
        FUIAction(FExecuteAction::CreateRaw(
        this, &FExtendMenuBaseModule::ExistingMainMenuEntryAction))// 绑定执行按钮的代理函数
        );
}

void FExtendMenuBaseModule::ExistingMainMenuEntryAction()
{
    UE_LOG(LogTemp, Warning, TEXT("ExistingMainMenuEntryAction is called."))
}


void FExtendMenuBaseModule::MakeNewMenuBarEntry(FMenuBarBuilder& MenuBarBuilder)
{
    MenuBarBuilder.AddPullDownMenu(						// 添加下拉菜单按钮
        FText::FromString("New Menu"),					// 该按钮名称
        FText::FromString("This is a new menu bar button"),
        FNewMenuDelegate::CreateRaw(this, &FExtendMenuBaseModule::MakeNewMainMenuEntry),
        "New Menu Hook"					                // 该按钮的ExtensionHook
        );
}

void FExtendMenuBaseModule::MakeNewMainMenuEntry(FMenuBuilder& MenuBuilder)
{
    MenuBuilder.BeginSection("Section 1", FText::FromString("Section 1"));	// 设置Section

    MenuBuilder.AddMenuEntry(
    FText::FromString("New Menu Button"),
    FText::FromString("This is a new menu button"),
    FSlateIcon(),
    FUIAction(FExecuteAction::CreateRaw(this, &FExtendMenuBaseModule::NewMainMenuEntryAction))
    );

    MenuBuilder.EndSection();	                                       // 结束Section设置
}

void FExtendMenuBaseModule::NewMainMenuEntryAction()
{
    UE_LOG(LogTemp, Warning, TEXT("NewMainMenuEntryAction is called."))
    
    // 创建新窗口
    TSharedRef<SWindow> Window = SNew(SWindow)
    .Title(FText::FromString("Alice Window"))
    .SizingRule(ESizingRule::UserSized)
    .ClientSize(FVector2D(1000.f, 700.f))
    .AutoCenter(EAutoCenter::PreferredWorkArea)
    .ScreenPosition(FVector2D(0, 0))
    .IsEnabled(true)
    .SupportsMinimize(true)
    .SupportsMaximize(true);
	
    // 设置窗口内容
    Window->SetContent
    (
        SNew(SConstraintCanvas)
        +SConstraintCanvas::Slot()
        .Alignment(0.5f)
        .Anchors(0.5f)
        .Offset(FMargin(0.0f, 0.0f, 300.f, 100.f))
        .AutoSize(true)
        [
            SNew(STextBlock)
            .Text(FText::FromString("Hello, Alice Window !"))
        ]
    );

    // 显示新窗口
    FSlateApplication::Get().AddWindow(Window);
}


#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FExtendMenuBaseModule, ExtendMenuBase)