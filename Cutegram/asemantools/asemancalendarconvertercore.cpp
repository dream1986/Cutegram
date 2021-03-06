/*
    Copyright (C) 2014 Aseman
    http://aseman.co

    This project is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This project is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "asemancalendarconvertercore.h"

#include <QObject>

int aseman_gregorian_months_start[13]      = {0,31,59,90,120,151,181,212,243,273,304,334,365};
int aseman_gregorian_leap_months_start[13] = {0,31,60,91,121,152,182,213,244,274,305,335,366};

int aseman_jalali_months_start[13]      = {0,31,62,93,124,155,186,216,246,276,306,336,365};
int aseman_jalali_leap_months_start[13] = {0,31,62,93,124,155,186,216,246,276,306,336,366};

int aseman_hijri_months_start[13]      = {0,30,59,89,118,148,177,207,236,266,295,325,354};
int aseman_hijri_leap_months_start[13] = {0,30,59,89,118,148,177,207,236,266,295,325,355};
int aseman_hijri_leap_years[11]        = {2,5,7,10,13,16,18,21,24,26,29};

class AsemanCalendarConverterCorePrivate
{
public:
    AsemanCalendarConverterCore::CalendarTypes calendar;
};

AsemanCalendarConverterCore::AsemanCalendarConverterCore()
{
    p = new AsemanCalendarConverterCorePrivate;
    p->calendar = AsemanCalendarConverterCore::Gregorian;
}

void AsemanCalendarConverterCore::setCalendar(AsemanCalendarConverterCore::CalendarTypes t)
{
    p->calendar = t;
}

AsemanCalendarConverterCore::CalendarTypes AsemanCalendarConverterCore::calendar() const
{
    return p->calendar;
}

QString AsemanCalendarConverterCore::paperString(const QDateTime &dt)
{
    const DateProperty & dp = getDate(dt.date());
    QString res = QString("%1, %2 %3 %4, %5").arg(dayName(dp.day_of_week)).arg(dp.day).arg(monthName(dp.month)).arg(dp.year).arg(dt.time().toString("hh:mm"));
    return res;
}

QString AsemanCalendarConverterCore::paperString(const QDateTime &d, const QString &format)
{
    const DateProperty & dp = getDate(d.date());
    QString res = format;
    res.replace("HH", QString::number(d.time().hour()).rightJustified(2,'0'));
    res.replace("hh", QString::number(d.time().hour()%12).rightJustified(2,'0'));
    res.replace("H", QString::number(d.time().hour()));
    res.replace("h", QString::number(d.time().hour()%12));
    res.replace("mm", QString::number(d.time().minute()).rightJustified(2,'0'));
    res.replace("m", QString::number(d.time().minute()));
    res.replace("ss", QString::number(d.time().second()).rightJustified(2,'0'));
    res.replace("s", QString::number(d.time().second()));
    res.replace("yyyy", QString::number(dp.year));
    res.replace("yy", QString::number(dp.year).right(2));
    res.replace("dddd", dayName(dp.day_of_week));
    res.replace("MMMM", monthName(dp.month));
    res.replace("dd", QString::number(dp.day).rightJustified(2,'0'));
    res.replace("MM", QString::number(dp.month).rightJustified(2,'0'));

    return res;
}

QString AsemanCalendarConverterCore::littleString(const QDate &d)
{
    const DateProperty & dp = getDate(d);
    QString res = QString("%1 %2 %3").arg(dp.day).arg(monthName(dp.month)).arg(dp.year);
    return res;
}

QString AsemanCalendarConverterCore::historyString(const QDate &d)
{
    const DateProperty & dp = getDate(d);
    QString res = QString("%1 %2 %3 - %4").arg(dp.year).arg(monthName(dp.month)).arg(dp.day).arg(dayName(dp.day_of_week));
    return res;
}

QString AsemanCalendarConverterCore::numberString(const QDate &d)
{
    const DateProperty & dp = getDate(d);
    QString res = QString("%1 %2 %3 - %4").arg(dp.year).arg(dp.month).arg(dp.day).arg(dayName(dp.day_of_week));
    return res;
}

DateProperty AsemanCalendarConverterCore::getDate(const QDate &d)
{
    DateProperty res;
    switch( static_cast<int>(p->calendar) )
    {
    case AsemanCalendarConverterCore::Gregorian:
        res = toDateGregorian( fromDateGregorian(d.year(),d.month(),d.day()) );
        break;
    case AsemanCalendarConverterCore::Jalali:
        res = toDateJalali( fromDateGregorian(d.year(),d.month(),d.day()) );
        break;
    case AsemanCalendarConverterCore::Hijri:
        res = toDateHijri( fromDateGregorian(d.year(),d.month(),d.day()) );
        break;
    }

    return res;
}

QDate AsemanCalendarConverterCore::toDate(qint64 y, int m, int d)
{
    qint64 julian_zero = 0;
    switch( static_cast<int>(p->calendar) )
    {
    case AsemanCalendarConverterCore::Gregorian:
        julian_zero = fromDateGregorian(y,m,d);
        break;
    case AsemanCalendarConverterCore::Jalali:
        julian_zero = fromDateJalali(y,m,d);
        break;
    case AsemanCalendarConverterCore::Hijri:
        julian_zero = fromDateHijri(y,m,d);
        break;
    }

    const DateProperty & pr = toDateGregorian(julian_zero);
    return QDate(pr.year,pr.month,pr.day);
}

QString AsemanCalendarConverterCore::dayName(int d)
{
    QString res;
    switch( static_cast<int>(p->calendar) )
    {
    case AsemanCalendarConverterCore::Gregorian:
        res = dayNameGregorian(d);
        break;
    case AsemanCalendarConverterCore::Jalali:
        res = dayNameJalali(d);
        break;
    case AsemanCalendarConverterCore::Hijri:
        res = dayNameHijri(d);
        break;
    }

    return res;
}

QString AsemanCalendarConverterCore::monthName(int m)
{
    QString res;
    switch( static_cast<int>(p->calendar) )
    {
    case AsemanCalendarConverterCore::Gregorian:
        res = monthNamesGregorian(m);
        break;
    case AsemanCalendarConverterCore::Jalali:
        res = monthNamesJalali(m);
        break;
    case AsemanCalendarConverterCore::Hijri:
        res = monthNamesHijri(m);
        break;
    }

    return res;
}

bool AsemanCalendarConverterCore::yearIsLeap(qint64 year)
{
    bool res = false;
    switch( static_cast<int>(p->calendar) )
    {
    case AsemanCalendarConverterCore::Gregorian:
        res = isLeapGregorian(year);
        break;
    case AsemanCalendarConverterCore::Jalali:
        res = isLeapJalali(year);
        break;
    case AsemanCalendarConverterCore::Hijri:
        res = leapIndexHijri(year) != -1;
        break;
    }

    return res;
}

int AsemanCalendarConverterCore::daysOfMonth(qint64 y, int m)
{
    if( m<1 || m>12 )
        return 0;

    int res = 0;
    bool leap = yearIsLeap(y);
    switch( static_cast<int>(p->calendar) )
    {
    case AsemanCalendarConverterCore::Gregorian:
        res = leap? aseman_gregorian_leap_months_start[m]-aseman_gregorian_leap_months_start[m-1] :
                aseman_gregorian_months_start[m]-aseman_gregorian_months_start[m-1];
        break;
    case AsemanCalendarConverterCore::Jalali:
        res = leap? aseman_jalali_leap_months_start[m]-aseman_jalali_leap_months_start[m-1] :
                aseman_jalali_months_start[m]-aseman_jalali_months_start[m-1];
        break;
    case AsemanCalendarConverterCore::Hijri:
        res = leap? aseman_hijri_leap_months_start[m]-aseman_hijri_leap_months_start[m-1] :
                aseman_hijri_months_start[m]-aseman_hijri_months_start[m-1];
        break;
    }

    return res;
}

bool AsemanCalendarConverterCore::isLeapGregorian( qint64 year )
{
    return (year%4==0 && year%100!=0) || year%400==0;
}

QString AsemanCalendarConverterCore::monthNamesGregorian(int m)
{
    switch( m )
    {
    case 1:
        return "January";
        break;
    case 2:
        return "February";
        break;
    case 3:
        return "March";
        break;
    case 4:
        return "April";
        break;
    case 5:
        return "May";
        break;
    case 6:
        return "June";
        break;
    case 7:
        return "July";
        break;
    case 8:
        return "August";
        break;
    case 9:
        return "September";
        break;
    case 10:
        return "October";
        break;
    case 11:
        return "November";
        break;
    case 12:
        return "December";
        break;
    }

    return QString();
}

QString AsemanCalendarConverterCore::dayNameGregorian(int d)
{
    switch( d )
    {
    case 1:
        return "Sunday";
        break;
    case 2:
        return "Monday";
        break;
    case 3:
        return "Tuesday";
        break;
    case 4:
        return "Wednesday";
        break;
    case 5:
        return "Thuresday";
        break;
    case 6:
        return "Friday";
        break;
    case 7:
        return "Saturday";
        break;
    }

    return QString();
}

qint64 AsemanCalendarConverterCore::fromDateGregorian( qint64 year , int month , int day )
{
    bool leap = isLeapGregorian( year );

    month--;
    day--;

    qint64 leap_pad = (year/4) - (year/100) + (year/400);
    qint64 year_days = year*365 + leap_pad;

    qint16 month_days = (leap)? aseman_gregorian_leap_months_start[month] : aseman_gregorian_months_start[month];
    qint64 abs_days   = year_days + month_days + day;
    if( year < 0 && !leap )
        abs_days--;

    return abs_days + 0;
}

DateProperty AsemanCalendarConverterCore::toDateGregorian( qint64 days_from_gregorian_zero )
{
    days_from_gregorian_zero -= 0;

    qint64 day     = days_from_gregorian_zero;
    qint64 year    = 0;
    qint16 month   = 0;

    year += day/146097 * 400;
    day   = day%146097;

    if( days_from_gregorian_zero < 0 && day != 0 )
    {
        year -= 400;
        day   = 146097 + day;
    }

    if( day < 36524*3 )
    {
        year += day/36524 * 100;
        day   = day%36524;
    }
    else
    {
        year += 400-100;
        day   = day - 36524*3;
    }

    if( day < 1461*24 )
    {
        year += day/1461 * 4;
        day   = day%1461;
    }
    else
    {
        year += 100-4;
        day   = day - 1461*24;
    }

    if( day < 365*3 )
    {
        year += day/365 * 1;
        day   = day%365;
    }
    else
    {
        year += 4-1;
        day   = day - 365*3;
    }

    day++;

    bool leap = isLeapGregorian(year);
    for( int i=11 ; i>=0 ; i-- )
    {
        qint16 month_day = (leap)? aseman_gregorian_leap_months_start[i] : aseman_gregorian_months_start[i] ;
        if( day > month_day )
        {
            month = i;
            day  -= month_day;
            break;
        }
    }

    month++;

    DateProperty property;
        property.day = day;
        property.month = month;
        property.year = year;
        property.day_of_week = (days_from_gregorian_zero) % 7;

    if( property.day_of_week < 0 )
        property.day_of_week = 6 + property.day_of_week;
    property.day_of_week++;

    return property;
}

bool AsemanCalendarConverterCore::isLeapJalali( qint64 year )
{
    return (year%4==0 && year%100!=0) || year%400==0;
}

QString AsemanCalendarConverterCore::monthNamesJalali(int m)
{
    switch( m )
    {
    case 1:
        return "Farvardin";
        break;
    case 2:
        return "Ordibehesht";
        break;
    case 3:
        return "Khordad";
        break;
    case 4:
        return "Tir";
        break;
    case 5:
        return "Mordad";
        break;
    case 6:
        return "Shahrivar";
        break;
    case 7:
        return "Mehr";
        break;
    case 8:
        return "Abaan";
        break;
    case 9:
        return "Aazar";
        break;
    case 10:
        return "Dey";
        break;
    case 11:
        return "Bahman";
        break;
    case 12:
        return "Esfand";
        break;
    }

    return QString();
}

QString AsemanCalendarConverterCore::dayNameJalali(int d)
{
    switch( d )
    {
    case 1:
        return "Shanbe";
        break;
    case 2:
        return "1Shanbe";
        break;
    case 3:
        return "2Shanbe";
        break;
    case 4:
        return "3Shanbe";
        break;
    case 5:
        return "4Shanbe";
        break;
    case 6:
        return "5Shanbe";
        break;
    case 7:
        return "Jome";
        break;
    }

    return QString();
}

qint64 AsemanCalendarConverterCore::fromDateJalali( qint64 year , int month , int day )
{
    bool leap = isLeapJalali( year );

    month--;
    day--;

    qint64 leap_pad = (year/4) - (year/100) + (year/400);
    qint64 year_days = year*365 + leap_pad;

    qint16 month_days = (leap)? aseman_jalali_leap_months_start[month] : aseman_jalali_months_start[month];
    qint64 abs_days   = year_days + month_days + day;
    if( year < 0 && !leap )
        abs_days--;

    return abs_days + 226894;
}

DateProperty AsemanCalendarConverterCore::toDateJalali( qint64 days_from_jalali_zero )
{
    days_from_jalali_zero -= 226894;

    qint64 day     = days_from_jalali_zero;
    qint64 year    = 0;
    qint16 month   = 0;

    year += day/146097 * 400;
    day   = day%146097;

    if( days_from_jalali_zero < 0 && day != 0 )
    {
        year -= 400;
        day   = 146097 + day;
    }

    if( day < 36524*3 )
    {
        year += day/36524 * 100;
        day   = day%36524;
    }
    else
    {
        year += 400-100;
        day   = day - 36524*3;
    }

    if( day < 1461*24 )
    {
        year += day/1461 * 4;
        day   = day%1461;
    }
    else
    {
        year += 100-4;
        day   = day - 1461*24;
    }

    if( day < 365*3 )
    {
        year += day/365 * 1;
        day   = day%365;
    }
    else
    {
        year += 4-1;
        day   = day - 365*3;
    }

    day++;

    bool leap = isLeapJalali(year);
    for( int i=11 ; i>=0 ; i-- )
    {
        qint16 month_day = (leap)? aseman_jalali_leap_months_start[i] : aseman_jalali_months_start[i] ;
        if( day > month_day )
        {
            month = i;
            day  -= month_day;
            break;
        }
    }

    month++;

    DateProperty property;
        property.day = day;
        property.month = month;
        property.year = year;
        property.day_of_week = (days_from_jalali_zero-3) % 7;

    if( property.day_of_week < 0 )
        property.day_of_week = 6 + property.day_of_week;
    property.day_of_week++;

    return property;
}

int AsemanCalendarConverterCore::leapIndexHijri( qint64 year )
{
    qint8 r = year%30;
    for( int i=0 ; i<11 ; i++ )
        if( r == aseman_hijri_leap_years[i] )
            return i;

    return -1;
}

QString AsemanCalendarConverterCore::monthNamesHijri( int m )
{
    switch( m )
    {
    case 1:
        return "Moharram";
        break;
    case 2:
        return "Safar";
        break;
    case 3:
        return "Rabiol Avval";
        break;
    case 4:
        return "Rabio Sani";
        break;
    case 5:
        return "Jamadiol Aval";
        break;
    case 6:
        return "Jamadio Sani";
        break;
    case 7:
        return "Rajab";
        break;
    case 8:
        return "Shaban";
        break;
    case 9:
        return "Ramadan";
        break;
    case 10:
        return "Shaval";
        break;
    case 11:
        return "Zighade";
        break;
    case 12:
        return "Zihaje";
        break;
    }

    return QString();
}

QString AsemanCalendarConverterCore::dayNameHijri(int d)
{
    switch( d )
    {
    case 1:
        return "Saturday";
        break;
    case 2:
        return "Sunday";
        break;
    case 3:
        return "Monday";
        break;
    case 4:
        return "Tuesday";
        break;
    case 5:
        return "Wednesday";
        break;
    case 6:
        return "Thuresday";
        break;
    case 7:
        return "Friday";
        break;
    }

    return QString();
}

qint64 AsemanCalendarConverterCore::leapsNumberHijri( qint64 year )
{
    qint8 r = year%30;
    for( int i=0 ; i<11 ; i++ )
        if( r <= aseman_hijri_leap_years[i] )
            return 11 * (year/30) + i + (r==aseman_hijri_leap_years[i]);

    return 0;
}

qint64 AsemanCalendarConverterCore::fromDateHijri( qint64 year , int month , int day )
{
    int leap_index = leapIndexHijri( year );
    bool leap = leap_index != -1;

    month--;
    day--;

    qint64 leap_pad = leapsNumberHijri(year);

    qint64 year_days = year*354 + leap_pad;

    qint16 month_days = (leap)? aseman_hijri_leap_months_start[month] : aseman_hijri_months_start[month];
    qint64 abs_days   = year_days + month_days + day;
    if( year < 0 && !leap )
        abs_days-=11;
    if( year >= 0 && leap )
        abs_days--;

    return abs_days+227026;
}

DateProperty AsemanCalendarConverterCore::toDateHijri( qint64 days_from_hijri_zero )
{
    days_from_hijri_zero -= 227026;

    qint64 day     = days_from_hijri_zero;
    qint64 year    = 0;
    qint16 month   = 0;

    year += day/10631 * 30;
    day   = day%10631;

    if( days_from_hijri_zero < 0 && day != 0 )
    {
        year -= 30;
        day   = 10631 + day;
    }

    for( int i=0 ; i<30 ; i++ )
    {
        int leap = leapIndexHijri( year );
        int year_days = (leap==-1)?354:355;
        if( day < year_days )
            break;

        year += 1;
        day  -= year_days;
    }

    day++;

    int leap_number = leapIndexHijri( year );
    bool leap = (leap_number!=-1);
    for( int i=11 ; i>=0 ; i-- )
    {
        qint16 month_day = (leap)? aseman_hijri_leap_months_start[i] : aseman_hijri_months_start[i] ;
        if( day > month_day )
        {
            month = i;
            day  -= month_day;
            break;
        }
    }

    month++;

    DateProperty property;
        property.day = day;
        property.month = month;
        property.year = year;
        property.day_of_week = (days_from_hijri_zero-4) % 7;

    if( property.day_of_week < 0 )
        property.day_of_week = 6 + property.day_of_week;
    property.day_of_week++;

    return property;
}

AsemanCalendarConverterCore::~AsemanCalendarConverterCore()
{
    delete p;
}
