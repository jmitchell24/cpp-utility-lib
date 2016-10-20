<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:msxsl="urn:schemas-microsoft-com:xslt" exclude-result-prefixes="msxsl"
>
    <xsl:output method="text" indent="no"/>

    <xsl:template match="table">
        <xsl:apply-templates select=".//tr"/>
    </xsl:template>

    <xsl:template match="tr">::util::ColorRGBA constexpr static <xsl:value-of select="td[2]"/> = ::util::colorByte( <xsl:value-of select="td[4]"/>,255 );
</xsl:template>

</xsl:stylesheet>
