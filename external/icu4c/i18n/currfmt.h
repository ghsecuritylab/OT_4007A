
#ifndef CURRENCYFORMAT_H
#define CURRENCYFORMAT_H

#include "unicode/utypes.h"

#if !UCONFIG_NO_FORMATTING

#include "unicode/measfmt.h"

U_NAMESPACE_BEGIN

class NumberFormat;

class CurrencyFormat : public MeasureFormat {

 public:

    /**
     * Construct a CurrencyFormat for the given locale.
     */
    CurrencyFormat(const Locale& locale, UErrorCode& ec);

    /**
     * Copy constructor.
     */
    CurrencyFormat(const CurrencyFormat& other);

    /**
     * Destructor.
     */
    virtual ~CurrencyFormat();

    /**
     * Override Format API.
     */
    virtual UBool operator==(const Format& other) const;

    /**
     * Override Format API.
     */
    virtual Format* clone() const;


    using MeasureFormat::format;

    /**
     * Override Format API.
     */
    virtual UnicodeString& format(const Formattable& obj,
                                  UnicodeString& appendTo,
                                  FieldPosition& pos,
                                  UErrorCode& ec) const;

    /**
     * Override Format API.
     */
    virtual void parseObject(const UnicodeString& source,
                             Formattable& result,
                             ParsePosition& pos) const;

    /**
     * Override Format API.
     */
    virtual UClassID getDynamicClassID() const;

    /**
     * Returns the class ID for this class.
     */
    static UClassID U_EXPORT2 getStaticClassID();

 private:

    NumberFormat* fmt;
};

U_NAMESPACE_END

#endif // #if !UCONFIG_NO_FORMATTING
#endif // #ifndef CURRENCYFORMAT_H
