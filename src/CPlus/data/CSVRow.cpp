#pragma once
#include <string>
#include <iostream>

class CSVRow {
public:
    std::string Country_Name;
    std::string Country_Code;
    std::string Indicator_Name;
    std::string Indicator_Code;
    std::string Year_1960;
    std::string Year_1961;
    std::string Year_1962;
    std::string Year_1963;
    std::string Year_1964;
    std::string Year_1965;
    std::string Year_1966;
    std::string Year_1967;
    std::string Year_1968;
    std::string Year_1969;
    std::string Year_1970;
    std::string Year_1971;
    std::string Year_1972;
    std::string Year_1973;
    std::string Year_1974;
    std::string Year_1975;
    std::string Year_1976;
    std::string Year_1977;
    std::string Year_1978;
    std::string Year_1979;
    std::string Year_1980;
    std::string Year_1981;
    std::string Year_1982;
    std::string Year_1983;
    std::string Year_1984;
    std::string Year_1985;
    std::string Year_1986;
    std::string Year_1987;
    std::string Year_1988;
    std::string Year_1989;
    std::string Year_1990;
    std::string Year_1991;
    std::string Year_1992;
    std::string Year_1993;
    std::string Year_1994;
    std::string Year_1995;
    std::string Year_1996;
    std::string Year_1997;
    std::string Year_1998;
    std::string Year_1999;
    std::string Year_2000;
    std::string Year_2001;
    std::string Year_2002;
    std::string Year_2003;
    std::string Year_2004;
    std::string Year_2005;
    std::string Year_2006;
    std::string Year_2007;
    std::string Year_2008;
    std::string Year_2009;
    std::string Year_2010;
    std::string Year_2011;
    std::string Year_2012;
    std::string Year_2013;
    std::string Year_2014;
    std::string Year_2015;
    std::string Year_2016;
    std::string Year_2017;
    std::string Year_2018;
    std::string Year_2019;
    std::string Year_2020;
    std::string Year_2021;
    std::string Year_2022;
    std::string Year_2023;

    CSVRow() = default;

    // Constructor to initialize from CSV data
    CSVRow(const std::string& countryName, const std::string& countryCode,
           const std::string& indicatorName, const std::string& indicatorCode,
           const std::string& year1960, const std::string& year1961, const std::string& year1962,
           const std::string& year1963, const std::string& year1964, const std::string& year1965,
           const std::string& year1966, const std::string& year1967, const std::string& year1968,
           const std::string& year1969, const std::string& year1970, const std::string& year1971,
           const std::string& year1972, const std::string& year1973, const std::string& year1974,
           const std::string& year1975, const std::string& year1976, const std::string& year1977,
           const std::string& year1978, const std::string& year1979, const std::string& year1980,
           const std::string& year1981, const std::string& year1982, const std::string& year1983,
           const std::string& year1984, const std::string& year1985, const std::string& year1986,
           const std::string& year1987, const std::string& year1988, const std::string& year1989,
           const std::string& year1990, const std::string& year1991, const std::string& year1992,
           const std::string& year1993, const std::string& year1994, const std::string& year1995,
           const std::string& year1996, const std::string& year1997, const std::string& year1998,
           const std::string& year1999, const std::string& year2000, const std::string& year2001,
           const std::string& year2002, const std::string& year2003, const std::string& year2004,
           const std::string& year2005, const std::string& year2006, const std::string& year2007,
           const std::string& year2008, const std::string& year2009, const std::string& year2010,
           const std::string& year2011, const std::string& year2012, const std::string& year2013,
           const std::string& year2014, const std::string& year2015, const std::string& year2016,
           const std::string& year2017, const std::string& year2018, const std::string& year2019,
           const std::string& year2020, const std::string& year2021, const std::string& year2022,
           const std::string& year2023)
        : Country_Name(countryName), Country_Code(countryCode), Indicator_Name(indicatorName),
          Indicator_Code(indicatorCode), Year_1960(year1960), Year_1961(year1961), Year_1962(year1962),
          Year_1963(year1963), Year_1964(year1964), Year_1965(year1965), Year_1966(year1966),
          Year_1967(year1967), Year_1968(year1968), Year_1969(year1969), Year_1970(year1970),
          Year_1971(year1971), Year_1972(year1972), Year_1973(year1973), Year_1974(year1974),
          Year_1975(year1975), Year_1976(year1976), Year_1977(year1977), Year_1978(year1978),
          Year_1979(year1979), Year_1980(year1980), Year_1981(year1981), Year_1982(year1982),
          Year_1983(year1983), Year_1984(year1984), Year_1985(year1985), Year_1986(year1986),
          Year_1987(year1987), Year_1988(year1988), Year_1989(year1989), Year_1990(year1990),
          Year_1991(year1991), Year_1992(year1992), Year_1993(year1993), Year_1994(year1994),
          Year_1995(year1995), Year_1996(year1996), Year_1997(year1997), Year_1998(year1998),
          Year_1999(year1999), Year_2000(year2000), Year_2001(year2001), Year_2002(year2002),
          Year_2003(year2003), Year_2004(year2004), Year_2005(year2005), Year_2006(year2006),
          Year_2007(year2007), Year_2008(year2008), Year_2009(year2009), Year_2010(year2010),
          Year_2011(year2011), Year_2012(year2012), Year_2013(year2013), Year_2014(year2014),
          Year_2015(year2015), Year_2016(year2016), Year_2017(year2017), Year_2018(year2018),
          Year_2019(year2019), Year_2020(year2020), Year_2021(year2021), Year_2022(year2022),
          Year_2023(year2023) {}

    // test printing
    void print() const {
        std::cout << "Country: " << Country_Name << ", Code: " << Country_Code
                  << ", Indicator: " << Indicator_Name << ", Year 1960: " << Year_1960 << std::endl;
    }
};
