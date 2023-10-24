## Example

`https://api.odpt.org/api/v4/odpt:TrainInformation?odpt:operator=odpt.Operator:JR-East`
`https://api.odpt.org/api/v4/odpt:TrainTimetable?odpt:operator=odpt.Operator:TokyoMetro&acl:consumerKey=2bbf82cd3d5647265c321eab3cd84ef0178ec8f4b472bf6655d13f4f11f92c67`

## Endpoint

`base_url`: `https://api.odpt.org/api/v4`

### path
- `/odpt:TrainInformation`
- `/odpt:TrainTimetable`


## Query
### `odpt:operator`
- `odpt.Operator:JR-East`
- `odpt.Operator:TokyoMetro`
### `acl:consumerKey`
- `2bbf82cd3d5647265c321eab3cd84ef0178ec8f4b472bf6655d13f4f11f92c67`
### `odpt:railway`
- `odpt.Railway:TokyoMetro.Chiyoda`
### `odpt:calendar`
- `odpt.Calendar:Weekday`
- `odpt.Calendar:SaturdayHoliday`







## Generate(metro.chiyoda.weekday)
### options
- endpoint: `odpt:TrainTimetable`
- `odpt:railway`: `odpt.Railway:TokyoMetro.Chiyoda`
- `odpt:calendar`: `odpt.Calendar:Weekday`
- `acl:consumerKey`: `2bbf82cd3d5647265c321eab3cd84ef0178ec8f4b472bf6655d13f4f11f92c67`

### url
Generated: [metro.chiyoda.weekday](https://api.odpt.org/api/v4/odpt:TrainTimetable?odpt:railway=odpt.Railway:TokyoMetro.Chiyoda&odpt:calendar=odpt.Calendar:Weekday&acl:consumerKey=2bbf82cd3d5647265c321eab3cd84ef0178ec8f4b472bf6655d13f4f11f92c67)

## Generate(metro.chiyoda.saturday_holiday)
### options
- endpoint: `odpt:TrainTimetable`
- `odpt:railway`: `odpt.Railway:TokyoMetro.Chiyoda`
- `odpt:calendar`: `odpt.Calendar:Holiday`
- `acl:consumerKey`: `2bbf82cd3d5647265c321eab3cd84ef0178ec8f4b472bf6655d13f4f11f92c67`

### url
Generated: [metro.chiyoda.saturday_holiday](https://api.odpt.org/api/v4/odpt:TrainTimetable?odpt:railway=odpt.Railway:TokyoMetro.Chiyoda&odpt:calendar=odpt.Calendar:SaturdayHoliday&acl:consumerKey=2bbf82cd3d5647265c321eab3cd84ef0178ec8f4b472bf6655d13f4f11f92c67)

## Generate(metro.hanzomon.weekday)
### options
- endpoint: `odpt:TrainTimetable`
- `odpt:railway`: `odpt.Railway:TokyoMetro.Hanzomon`
- `odpt:calendar`: `odpt.Calendar:Weekday`
- `acl:consumerKey`: `2bbf82cd3d5647265c321eab3cd84ef0178ec8f4b472bf6655d13f4f11f92c67`

### url
Generated: [metro.hanzomon.weekday](https://api.odpt.org/api/v4/odpt:TrainTimetable?odpt:railway=odpt.Railway:TokyoMetro.Hanzomon&odpt:calendar=odpt.Calendar:Weekday&acl:consumerKey=2bbf82cd3d5647265c321eab3cd84ef0178ec8f4b472bf6655d13f4f11f92c67)

## Generate(metro.hanzomon.saturday_holiday)
### options
- endpoint: `odpt:TrainTimetable`
- `odpt:railway`: `odpt.Railway:TokyoMetro.Hanzomon`
- `odpt:calendar`: `odpt.Calendar:Holiday`
- `acl:consumerKey`: `2bbf82cd3d5647265c321eab3cd84ef0178ec8f4b472bf6655d13f4f11f92c67`

### url
Generated: [metro.hanzomon.saturday_holiday](https://api.odpt.org/api/v4/odpt:TrainTimetable?odpt:railway=odpt.Railway:TokyoMetro.Hanzomon&odpt:calendar=odpt.Calendar:SaturdayHoliday&acl:consumerKey=2bbf82cd3d5647265c321eab3cd84ef0178ec8f4b472bf6655d13f4f11f92c67)





# get all railway
https://api.odpt.org/api/v4/odpt:Railway?odpt:operator=odpt.Operator:TokyoMetro&acl:consumerKey=2bbf82cd3d5647265c321eab3cd84ef0178ec8f4b472bf6655d13f4f11f92c67