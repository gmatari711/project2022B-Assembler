פרוייקט גמר מעבדה בתכנות מערכות ב Ansi C - אסמבלר 
שם: גל מטרי
ת.ז: 300599909

הארכה בהגשת הממ"ן:
מכיוון ששירתתי בסמסטר האביב בחודש מילואים ובנוסף הייתי חולה בתקופת הגשת הפרוייקט, קבלתי אישור חריג ממיכל להגשה באיחור של שבוע ללא הורדת ציון
(הסתיים ב28) ובנוסף שבוע איחור עם הורדה של 10 נקודות ביחס ליום ההגשה (יסתיים ב4.9).


חלקי הפרוייקט:
הפרוייקט חולק למודולים לפי פונקציונליות וכמובן לאבני בניין של מבני נתונים מופשטים.

word:
אבן בניין די מרכזית, למרות היותה מזערית. 
מדובר כאן בהפשטה של המושג מילת זיכרון והנגשתו ע"י העברת ערכים שלמים ובניית בלוקים באמצעות פונקציות API.
כמה שנסתיר יותר את מושג מילת הזיכרון, יהיה יותר קל לעדכן אותו בהמשך ולבצע בו שינויים (*אתיחס בהמשך).
סך הכל המבנה פשוט וברור ואין הרבה מה להסביר ולהרחיב.

parse_data:
המודול המרכזי של הפרוייקט הוא בעייני parse_data.
חלק זה דרש יותר יצירתיות משאר חלקי התכנית.
האתגר המרכזי שלי היה להפריד את מימוש הדברים מהשימוש החיצוני, בעצם מדובר עקרון הכימוס, על מנת לאפשר גבולות ברורים בין המודולים ומובן לא
ליצר קוד תלותי. כאן נכנס לשימוש מבנה שהוא מעין פסאודו סינגלטון בשם ParsedData, אשר מאחסן את כל המידע הדרוש לאחר ניתוח שורת אסמבלי.
המימוש של ParsedData מוסתר לחלוטין מהמשתמש ובכך מאפשר לו רק לבקש את המידע הדרוש בלי לדאוג ללוגיקה שמאחורי הדברים, מכיוון שזו 
יכולה לעיתים (והייתה) לא נאה ואינטואיטיבית במיוחד למשתמש החיצוני.
נעשה במבנה שימוש באותם שדות למספר נתונים, כאשר הקונטקסט לשימוש הוא שקובע מה יהיה קיים ברגע נתון. ביצעתי הגנות גישה באמצעות assert
על מנת להבטיח שאין שימוש במידע שכביכול לא נקרא בשורה הנכחית.

רוב המידע לגבי הניתוח הוא די מובן מאליו, מדובר בIf-ים מקוננים וswitch-ים למיניהם. אין פה משהו מיוחד. החלק המשמעותי כאן היה בהכנה לניתוח.
בגלל שידעתי אורך שורה מקסימלי, יכלתי להרשות לעצמי ולבצע ניתוח מקדים לשורת הטקסט ולהוסיף רווחים לפני ואחרי כל פסיק ובכך בעצם לייצר
באמצעות שימוש בstrtok עם תוחם של תווים לבנים מערך מחרוזות שבו כל תא הוא מילה יחידה או פסיק. מכאן הניתוח הוא מאוד פשוט.
הסיבה שבחרתי במימוש הזה, הוא העובדה שאני יודע את אורך השורה הצפוי ולכן יכול לאמוד את העלות מול תועלת מבחינת זיכרון, במקרה זה די זניחה,
והסיבה השניה היא שזה מקל מאוד על בחירת החוקים לבניית תחביר האסמבלי ( אתיחס לזה בסוף הקובץ).

מבני הנתונים:
השתמשתי בשלושה אבני בניין גנריים מרכזיים למבני הנתונים שלי:
1) רשימה מקושרת - ממומשת עם זקיפים לראש ולזנב על מנת להקל על תהליך השרשור ולאפשר מימוש קל ונוח לאיטרטורים.
2) איטרטורים לרשימה מקושרת - אפשרו לי לעבור בצורה לינארית בין הנתונים בלי לחשוף מימוש ולפגוע בהפשטה.
3) טבלת גיבוב - מקצרת משמעותית את זמן הגישה לתוויות מאקרו וסמלים.

ממבני הנתונים האלה בניתי את רשימות הentry והextern , את התור לאחסון שגיאות ואת טבלאות הסמלים והמאקרו.

**גילוי נאות ** - חוויתי בעיה עם המבנים הגנריים שלי בשלבים האחרונים של ניפוי השגיאות. אלה מבנים רגולרים שאני עובד אתם המון זמן ומעולם לא חוויתי אתם
בעיות מהסוג הזה ועדיין לא הצלחתי למצוא את הפתרון. כרגע הזכרון מטבלאות הגיבוב לא עובר שחרור כדי להמנע מ-segmentation fault.
אני מחשיב את עצמי כמפנה יחסית טוב ברמת המימוש שעשינו בקורס ועדיין לא הצלחתי למצוא את הבעיה לא משנה באילו כלים נעזרתי. 
ייתכן שאני עמוק בפנים ולכן לא יכול לראות. אפתור את זה בהקדם מכיוון שזה מבחינתי פרוייקט נהדר להצגה.

Pre_assembler:
נשענתי על העובדה שלא צריך לבצע ניפוי שגיאות על מנת לבצע קדם אסמבלר במעבר יחיד (צויין מפורשות בהוראות הממ"ן וגם בסרטון ההדרכה).
כדי לקצר מעט את החיפוש השתמשתי בטבלאת גיבוב עם חוקיות פשוטה - האות הראשונה של התווית קובעת את האינדקס לפי מיקומה באלפא בית האמריקאי.
בגדול, לקחתי בחשבון שלא נגיע כאן ליעילות שיא, בסוף בהתפלגות אחידה מדובר כאן ב- N חלקי 26. יכלתי להכפיל את כמות התאים והייתי מרוויח מעט יותר 
על ידי שימו נפרד בlower ו upper, אך זה זניח, או לחלופין להגדיל אף יותר ולהשתמש בפונקציית גיבוב חזקה יותר - הרגיש לי מעט overkill.
ברור שמדובר פה יותר בתצוגת תחליט מאשר אפקט ממשי, אבל הפרוייקט חשוב לי ואמשיך לשפר אותו אחרי ההגשה כשאסיים עם העומס במקצועות הקיץ.
העבודה של הקדם אסמבלר די פשוטה -
קריאת שורת טקסט - אם אותר מאקרו - מייצרים אובייקט מאקרו וקוראים אליו את כל שורות הקוד הרלבנטיות. אם במקום זאת המילה הראשונה היא כותרת מאקרו
ואם כן מדפיסים במקומה לקובץ החדש את כל השורות המאוחסנות באובייקט המאקרו הנמצא. אחרת, נכתוב לקובץ הפלט את השורה הנכחית בשלמותה.

first_pass
מייצר מופע של ParsedData באמצעות הפונקצייה process_command וגוזר ממנו את כל הנתונים הדרושים באמצעות פונקציות API נתונות ובונה את טבלאת
הסמלים, תמונת הקוד ותמונת הנתונים.
אם קיימת שגיאה, היא נכנסת לתור השגיאות ומרגע זה אנו מפסיקים את הבנייה של טבלאת הנתונים, תמונת הקוד ותמונת הנתונים ורק מחפשים בעיות 
סינטקטיות היכן שקיימות ואם קיימות.
השיקול שלי היה לשמור על קו אחיד עם התפיסה שאם נמצאו שגיאות, לא עוברים למעבר השני ולא מוצאים את הבעיות הקשורות בחוסרים בטבלאת הסמלים
מכיוון שבניה זו היא מיותרת. לכן באותו הקו החלטתי שבעיות הקשורות בטבלאת הסמלים לא יטופלו לאחר שנמצאו שגיאות, אלא רק ניפוי שגיאות סינטקטיות
וסמנטיות (שגם ככה נמצאים בשלב הprocess_command).
ניתן לראות לעיתים קרובות גם בניפוי שגיאות של של הgcc שלאחר פתרון בעיות סינטקיטיות בסיסיות אנו מגלים בעיות פנימיות שלא יודענו עליהם לפני.

second_pass
בשלב זה התכנית ממשיכה את ניפוי השגיאות (הקשורות בסמלים בלבד), מעדכנת את תמונת הקוד באמצעות הסמלים שלא עודכנו בסבב הראשון וכמובן
ממלאת את רשימת הextern ורשימת הentry.
חלק זה קצר משמעותית, אך בעייני העבודה בו היא עדינה יותר ויש לחשוב היטב על הדיוק על מנת לא לדרוס מידע קיים.

file_builder
מכיל פונקציות לבנייתי קבצים עם סיומות רצויות בהינתן שם וכמובן פונקציות לייצור קבצי הפלט ob, ent, ext.
מודול קצר מאוד ודי מובן מאליו.

instruction_encoding
מכיל את כללי הקידוד של ההוראות ומאפשר לנו לקודד את המידע הדרוש לshort int כמשתנה פלט.
השתמשתי פה בunionים ושדות סיביות.
**גילוי נאות** בעבר לא הערכתי את שני השימושים האלה כראוי. כיום אני מעריך אותם עד אין קץ.

misc
מכיל הגדרות כלליות לשימו התכנית באופן הרחב ביותר, כמו הגדרת משתנה בוליאני.
גם חלק זה מובן וברור.


הערות לגבי המימושים בפרוייקט:
התיחסות להערה בשלב המילה - בדיעבד הייתי מבצע את כל העבודה עם מילים בקוד עם int ולא short. אני נשענתי על העובדה שאנו יודעים את גודל מילת הקוד. אמנם
לא חשפתי מימוש, אך זה די קרוב לכך.

במידה והיה לי מעט יותר זמן, או לחלופין אם ביממה היו 30 שעות, הייתי מפשט את החלק של parse_data בצורה אבסולוטית. הייתי מייצר הפרדה של החוקים והכללים מהניתוח
על ידי שימוש בפונקציות חיצוניות. כלומר, בהנחה שאבני הבניין קבועים אך הסינטקס שונה, הייתי מקבל את הסינטקס כפונקציות חיצונים.
זה היה דורש ממני עוד מספר ימים של תכנון ועבודה אבל בהחלט היה מרים את הפרוייקט לרמה טובה יותר.

הערות כלליות:
הפרוייקט הוא אב טיפוס לתוצר שהייתי רוצה. לצערי לא היה לי את הזמן לבנות מחדש את מבני הנתונים ש"קיצרו" לי בשלבי הניפוי האחרונים. אמנם אני יכול
להגיש עד יום ראשון, אבל בפועל יש לי סמסטר קיץ עמוס במטלות ומבחנים שפספסתי מסמסטר אביב בגלל המילואים. תערבב את זה עם העובדה שאין מסגרות עד ה4.9 ונקבל
הרבה אין זמן.
מאוד נהנתי מלעשות את הפרוייקט ובעיקר מלתכנן אותו. מדובר בלקחת חזון מ-0 ולראות אותו מתפתח למוצר מוגמר וזו תמיד תחושה טובה.
למדתי המון. יישמתי המון. התאהבתי שוב בשפת ansi c ונזכרתי כמה היא עוצמתית ויפה.
אני מגיש מוצר לא מוכן, אך לפי 3097 הטסטים שהרצתי - עובד. מניח שיש עוד הערות חסרות, גימורים ועידונים שצריך לעשות. אשמח לקבל כל הערה וכמובן שאחרי שאסיים עם המירוץ
הנורא של השלמות על השלמות באדיבות צה"ל, אסיים אותו כראוי בשביל הפורטפוליו.

נ.ב: אם תזהה במהלך הקוד במקרה מדוע מתקבל segfault וגם תזכור את השורה הזו, אשמח להערה והארה בנושא.
בניתיים אתעסק בלכתוב את המבנים מחדש.

יבורך הפרי.
גל מטרי