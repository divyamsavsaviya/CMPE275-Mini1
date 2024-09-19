class CSVRow:
    def __init__(self, **kwargs):
        self.Country_Name = kwargs.get('Country Name')
        self.Country_Code = kwargs.get('Country Code')
        self.Indicator_Name = kwargs.get('Indicator Name')
        self.Indicator_Code = kwargs.get('Indicator Code')
        self.Year_1960 = kwargs.get('1960')
        self.Year_1961 = kwargs.get('1961')
        self.Year_1962 = kwargs.get('1962')
        self.Year_1963 = kwargs.get('1963')
        self.Year_1964 = kwargs.get('1964')
        self.Year_1965 = kwargs.get('1965')
        self.Year_1966 = kwargs.get('1966')
        self.Year_1967 = kwargs.get('1967')
        self.Year_1968 = kwargs.get('1968')
        self.Year_1969 = kwargs.get('1969')
        self.Year_1970 = kwargs.get('1970')
        self.Year_1971 = kwargs.get('1971')
        self.Year_1972 = kwargs.get('1972')
        self.Year_1973 = kwargs.get('1973')
        self.Year_1974 = kwargs.get('1974')
        self.Year_1975 = kwargs.get('1975')
        self.Year_1976 = kwargs.get('1976')
        self.Year_1977 = kwargs.get('1977')
        self.Year_1978 = kwargs.get('1978')
        self.Year_1979 = kwargs.get('1979')
        self.Year_1980 = kwargs.get('1980')
        self.Year_1981 = kwargs.get('1981')
        self.Year_1982 = kwargs.get('1982')
        self.Year_1983 = kwargs.get('1983')
        self.Year_1984 = kwargs.get('1984')
        self.Year_1985 = kwargs.get('1985')
        self.Year_1986 = kwargs.get('1986')
        self.Year_1987 = kwargs.get('1987')
        self.Year_1988 = kwargs.get('1988')
        self.Year_1989 = kwargs.get('1989')
        self.Year_1990 = kwargs.get('1990')
        self.Year_1991 = kwargs.get('1991')
        self.Year_1992 = kwargs.get('1992')
        self.Year_1993 = kwargs.get('1993')
        self.Year_1994 = kwargs.get('1994')
        self.Year_1995 = kwargs.get('1995')
        self.Year_1996 = kwargs.get('1996')
        self.Year_1997 = kwargs.get('1997')
        self.Year_1998 = kwargs.get('1998')
        self.Year_1999 = kwargs.get('1999')
        self.Year_2000 = kwargs.get('2000')
        self.Year_2001 = kwargs.get('2001')
        self.Year_2002 = kwargs.get('2002')
        self.Year_2003 = kwargs.get('2003')
        self.Year_2004 = kwargs.get('2004')
        self.Year_2005 = kwargs.get('2005')
        self.Year_2006 = kwargs.get('2006')
        self.Year_2007 = kwargs.get('2007')
        self.Year_2008 = kwargs.get('2008')
        self.Year_2009 = kwargs.get('2009')
        self.Year_2010 = kwargs.get('2010')
        self.Year_2011 = kwargs.get('2011')
        self.Year_2012 = kwargs.get('2012')
        self.Year_2013 = kwargs.get('2013')
        self.Year_2014 = kwargs.get('2014')
        self.Year_2015 = kwargs.get('2015')
        self.Year_2016 = kwargs.get('2016')
        self.Year_2017 = kwargs.get('2017')
        self.Year_2018 = kwargs.get('2018')
        self.Year_2019 = kwargs.get('2019')
        self.Year_2020 = kwargs.get('2020')
        self.Year_2021 = kwargs.get('2021')
        self.Year_2022 = kwargs.get('2022')
        self.Year_2023 = kwargs.get('2023')

    def __repr__(self):
        return (f"CSVRow(Country_Name={self.Country_Name}, Country_Code={self.Country_Code}, "
                f"Indicator_Name={self.Indicator_Name}, Indicator_Code={self.Indicator_Code}, "
                f"Year_1960={self.Year_1960}, Year_1961={self.Year_1961}, Year_1962={self.Year_1962}, "
                f"Year_1963={self.Year_1963}, Year_1964={self.Year_1964}, Year_1965={self.Year_1965}, "
                f"Year_1966={self.Year_1966}, Year_1967={self.Year_1967}, Year_1968={self.Year_1968}, "
                f"Year_1969={self.Year_1969}, Year_1970={self.Year_1970}, Year_1971={self.Year_1971}, "
                f"Year_1972={self.Year_1972}, Year_1973={self.Year_1973}, Year_1974={self.Year_1974}, "
                f"Year_1975={self.Year_1975}, Year_1976={self.Year_1976}, Year_1977={self.Year_1977}, "
                f"Year_1978={self.Year_1978}, Year_1979={self.Year_1979}, Year_1980={self.Year_1980}, "
                f"Year_1981={self.Year_1981}, Year_1982={self.Year_1982}, Year_1983={self.Year_1983}, "
                f"Year_1984={self.Year_1984}, Year_1985={self.Year_1985}, Year_1986={self.Year_1986}, "
                f"Year_1987={self.Year_1987}, Year_1988={self.Year_1988}, Year_1989={self.Year_1989}, "
                f"Year_1990={self.Year_1990}, Year_1991={self.Year_1991}, Year_1992={self.Year_1992}, "
                f"Year_1993={self.Year_1993}, Year_1994={self.Year_1994}, Year_1995={self.Year_1995}, "
                f"Year_1996={self.Year_1996}, Year_1997={self.Year_1997}, Year_1998={self.Year_1998}, "
                f"Year_1999={self.Year_1999}, Year_2000={self.Year_2000}, Year_2001={self.Year_2001}, "
                f"Year_2002={self.Year_2002}, Year_2003={self.Year_2003}, Year_2004={self.Year_2004}, "
                f"Year_2005={self.Year_2005}, Year_2006={self.Year_2006}, Year_2007={self.Year_2007}, "
                f"Year_2008={self.Year_2008}, Year_2009={self.Year_2009}, Year_2010={self.Year_2010}, "
                f"Year_2011={self.Year_2011}, Year_2012={self.Year_2012}, Year_2013={self.Year_2013}, "
                f"Year_2014={self.Year_2014}, Year_2015={self.Year_2015}, Year_2016={self.Year_2016}, "
                f"Year_2017={self.Year_2017}, Year_2018={self.Year_2018}, Year_2019={self.Year_2019}, "
                f"Year_2020={self.Year_2020}, Year_2021={self.Year_2021}, Year_2022={self.Year_2022}, "
                f"Year_2023={self.Year_2023})")