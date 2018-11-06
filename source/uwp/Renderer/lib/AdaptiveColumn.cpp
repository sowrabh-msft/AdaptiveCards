#include "pch.h"
#include "AdaptiveColumn.h"

#include "Util.h"
#include "Vector.h"
#include <windows.foundation.collections.h>

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::AdaptiveNamespace;
using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::UI::Xaml;
using namespace ABI::Windows::UI::Xaml::Controls;

namespace AdaptiveNamespace
{
    AdaptiveColumn::AdaptiveColumn() { m_items = Microsoft::WRL::Make<Vector<IAdaptiveCardElement*>>(); }

    HRESULT AdaptiveColumn::RuntimeClassInitialize() noexcept try
    {
        std::shared_ptr<AdaptiveSharedNamespace::Column> column = std::make_shared<AdaptiveSharedNamespace::Column>();
        return RuntimeClassInitialize(column);
    }
    CATCH_RETURN;

    _Use_decl_annotations_ HRESULT AdaptiveColumn::RuntimeClassInitialize(const std::shared_ptr<AdaptiveSharedNamespace::Column>& sharedColumn) try
    {
        GenerateContainedElementsProjection(sharedColumn->GetItems(), m_items.Get());
        GenerateActionProjection(sharedColumn->GetSelectAction(), &m_selectAction);

        m_style = static_cast<ABI::AdaptiveNamespace::ContainerStyle>(sharedColumn->GetStyle());
        m_verticalAlignment =
            static_cast<ABI::AdaptiveNamespace::VerticalContentAlignment>(sharedColumn->GetVerticalContentAlignment());

        RETURN_IF_FAILED(UTF8ToHString(sharedColumn->GetWidth(), m_width.GetAddressOf()));
        m_pixelWidth = sharedColumn->GetPixelWidth();
        RETURN_IF_FAILED(MakeAndInitialize<AdaptiveBackgroundImage>(m_backgroundImage.GetAddressOf(),
                                                                    sharedColumn->GetBackgroundImage()));
        InitializeBaseElement(std::static_pointer_cast<BaseCardElement>(sharedColumn));
        return S_OK;
    }
    CATCH_RETURN;

    _Use_decl_annotations_ HRESULT AdaptiveColumn::get_Width(HSTRING* width) { return m_width.CopyTo(width); }

    _Use_decl_annotations_ HRESULT AdaptiveColumn::put_Width(HSTRING width) { return m_width.Set(width); }

    _Use_decl_annotations_ HRESULT AdaptiveColumn::get_PixelWidth(UINT32* pixelWidth)
    {
        return *pixelWidth = m_pixelWidth;
    }

    _Use_decl_annotations_ HRESULT AdaptiveColumn::put_PixelWidth(UINT32 pixelWidth)
    {
        return m_pixelWidth = pixelWidth;
    }

    _Use_decl_annotations_ HRESULT AdaptiveColumn::get_Style(ABI::AdaptiveNamespace::ContainerStyle* style)
    {
        *style = m_style;
        return S_OK;
    }

    _Use_decl_annotations_ HRESULT AdaptiveColumn::put_Style(ABI::AdaptiveNamespace::ContainerStyle style)
    {
        m_style = style;
        return S_OK;
    }

    _Use_decl_annotations_ HRESULT AdaptiveColumn::get_VerticalContentAlignment(ABI::AdaptiveNamespace::VerticalContentAlignment* verticalAlignment)
    {
        *verticalAlignment = m_verticalAlignment;
        return S_OK;
    }

    _Use_decl_annotations_ HRESULT AdaptiveColumn::put_VerticalContentAlignment(ABI::AdaptiveNamespace::VerticalContentAlignment verticalAlignment)
    {
        m_verticalAlignment = verticalAlignment;
        return S_OK;
    }

    _Use_decl_annotations_ HRESULT AdaptiveColumn::get_BackgroundImage(IAdaptiveBackgroundImage** backgroundImage)
    {
        return m_backgroundImage.CopyTo(backgroundImage);
    }

    _Use_decl_annotations_ HRESULT AdaptiveColumn::put_BackgroundImage(IAdaptiveBackgroundImage* backgroundImage)
    {
        m_backgroundImage = backgroundImage;
        return S_OK;
    }

    _Use_decl_annotations_ HRESULT AdaptiveColumn::get_Items(IVector<IAdaptiveCardElement*>** items)
    {
        return m_items.CopyTo(items);
    }

    _Use_decl_annotations_ HRESULT AdaptiveColumn::get_SelectAction(IAdaptiveActionElement** action)
    {
        return m_selectAction.CopyTo(action);
    }

    _Use_decl_annotations_ HRESULT AdaptiveColumn::put_SelectAction(IAdaptiveActionElement* action)
    {
        m_selectAction = action;
        return S_OK;
    }

    _Use_decl_annotations_ HRESULT AdaptiveColumn::get_ElementType(ElementType* elementType)
    {
        *elementType = ElementType::Column;
        return S_OK;
    }

    HRESULT AdaptiveColumn::GetSharedModel(std::shared_ptr<AdaptiveSharedNamespace::BaseCardElement>& sharedModel) try
    {
        std::shared_ptr<AdaptiveSharedNamespace::Column> column = std::make_shared<AdaptiveSharedNamespace::Column>();
        RETURN_IF_FAILED(SetSharedElementProperties(std::static_pointer_cast<AdaptiveSharedNamespace::BaseCardElement>(column)));

        column->SetStyle(static_cast<AdaptiveSharedNamespace::ContainerStyle>(m_style));
        column->SetVerticalContentAlignment(static_cast<AdaptiveSharedNamespace::VerticalContentAlignment>(m_verticalAlignment));
        column->SetWidth(HStringToUTF8(m_width.Get()));
        column->SetPixelWidth(m_pixelWidth);

        std::shared_ptr<AdaptiveSharedNamespace::BackgroundImage> sharedBackgroundImage;
        auto backgroundImage = static_cast<AdaptiveNamespace::AdaptiveBackgroundImage*>(m_backgroundImage.Get());
        RETURN_IF_FAILED(backgroundImage->GetSharedModel(sharedBackgroundImage));
        column->SetBackgroundImage(sharedBackgroundImage);

        if (m_selectAction != nullptr)
        {
            std::shared_ptr<BaseActionElement> sharedAction;
            RETURN_IF_FAILED(GenerateSharedAction(m_selectAction.Get(), sharedAction));
            column->SetSelectAction(sharedAction);
        }

        GenerateSharedElements(m_items.Get(), column->GetItems());

        sharedModel = column;
        return S_OK;
    }
    CATCH_RETURN;
}
