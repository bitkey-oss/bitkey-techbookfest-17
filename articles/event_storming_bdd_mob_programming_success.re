= 「イベントストーミング」x「BDD」をモブプロで実践したらめっちゃ良かった！

//flushright{
takuuuuuuu777

@takuuuuuuu777
//}

== はじめに

「イベントストーミング」x「BDD / TDD」x「DDD」x「モブプロ / ペアプロ」を活用して開発プロセスの改善を試みた結果、得られた知見と個人の所感をまとめた記事になります。

実際に活用してみて得られた成果・知見、シナジーについてまとめています。

=== 対象読者

 * イベントストーミングやBDDに興味がある方
 * モブプロやTDDを使ってより良いプロセスに昇華させたい方
 * 実装チームだけでなく、ビジネスステークホルダーやQAチーム、UIUXチームなどと強調して開発を進めたい方
 * 開発と議論しながら評価を進めたいと思っているQAの方

=== 記事を読むメリット

 * イベントストーミングやBDD、モブプロを取り入れた結果どうなるのか、１つの事例を知ることができ、活用の判断材料とすることができる！
 * 実施にあたり工夫すると良い点や、考慮しておくべきポイントを事前に知ることができる！

== 登場人物

「イベントストーミング」や「BDD」をモブプロで実践する前後で、開発プロセスがどう変化したかについて記載していきます。…が、開発プロセスを記載する前に、登場人物を簡単に説明しておきます。記載や役割はビットキー社内で使っているものとなります。

//image[2024-07-14_1_16_39][登場人物]{

//}

 * PdM
    
    Product Manager。プロダクトとして市場に提供するべき価値やその方向性を定め、いつどのような機能を提供するかの意思決定をします。実装をすることは少なく、ロードマップの策定やスプリントゴールの設定や、スプリントタスクの優先度判断などを主として行います。
    
 * CX
    
    Customer Experience。利用している(これから利用する)ユーザーへのより高い価値提供するために、お客様の利用目的や運用をヒアリングし、活用方法を提案したり、既存機能で不足している場合は、開発部門へ機能開発を要望したりする。
    
 * 開発
    
    機能の設計から実装を行い、プロダクトを実際に作り改善を続けることで価値提供を継続的に行う。単に実装をするだけでなく、特にシステム的な観点でお客様に価値提供しつづけるためには、設計はもちろん提供予定の機能が適切か漏れがないか精査したり、時には機能の提案たりも実施する。
    中でも技術に長けてチームをリードするメンバーをTL(テックリード)と呼称します。
    
 * QA
    
    Quality Assurance。提供予定の価値を提供できるか~~しきれるか~~、様々な観点で評価し保証できる状態とします。単に開発が作ったものが欠陥なく仕様通りかをチェックするだけでなく、実際にプロダクトを利用するユーザーが満足度高く利用することができるか幅広い観点でチェックし、時には提案も行います。
    
 * UI/UX
    
    User Interface / User Experience。提供したい価値を適切に届けるために、プロダクトを通してユーザーにどのような体験を提供するかを設計し、プロダクトのインターフェース(≒デザイン)を決定する。
    

== Before

=== 開発プロセス

改善前の開発プロセスです。

全体の図とステップごとの簡単な説明を記述しています。

//image[2024-07-18_9_15_51][開発プロセス]{

//}

==== 凡例

//image[2024-07-14_2_48_23][作業内容][scale=0.6]{

//}

//image[2024-07-14_2_48_51][分割された作業内容][scale=0.6]{

//}

=== プロセス詳細

==== 価値設計 (Product Value Design)
    
    現状の課題を踏まえ誰にどんな価値を提供するかを設計し、どのような機能を開発するかを決定します。本当にマーケットの課題を解決できるのかユーザーに価値を提供できるのか、CXと議論して進めます。
    誰にどんな価値を提供するか…が主要素であるため、これを実現するためのオペレーションフローまでは詳細化されない場合が多いです。
    完了次第、開発メンバーやQA、UI/UXに内容を共有します。
    
    ex.「マンションの入居者が Suica でエントランスも解錠できるようにしたい」など
    
==== 設計 & デザイン
    
    開発メンバーが設計を実施します。新規画面があるなどデザインが必要な要素がある場合は、UI/UXチームがデザインを実施します。デザインされた内容が実現可能か否かについては適宜、開発メンバー, PdMとコミュニケーションとりながら進めます。
    設計された「価値」を実現するための詳細を精査します。オペレーションやシステム的な考慮点の漏れがないかのチェック、漏れがあった場合には必要に応じて開発メンバーがPdM, CX とコミュニケーションとって詳細を整理します。
    設計 & デザインは開発メンバー間で共有 & FBを行い懸念や不明点が無くなった時点で完了です。完了し次第SPをメンバーで見積もってバックログに積みます。
    完了し次第、開発メンバーやQA、UI/UXに内容を共有します。
    
==== 実装
    
    スプリントプランニングで設計 & デザイン済みのストーリーをメンバーにそれぞれアサインした後、スプリントタスクとして実施していきます。
    必要に応じて着手時点でタスク分解して複数メンバーで分担しながら実施していきます。
    実装時点で、追加で対応しなければならないことや考慮漏れが発覚した場合には、PdMやCX, UI/UX と相談して対応方針を決めます。場合によっては別タスクとして切り出して翌スプリント移行での対応とします。
    実装後はTLのレビューを受けた後マージして完了です。
    開発が完了したタイミング、またはそれ以前にシステムの仕様を必要に応じてPdM、QAに共有します。詳細はシステムの挙動や制御は実装中に変わることが多く、実装しながらまたは完了後に都度情報の共有をしています。機能を利用する際に留意すべきことなどできるだけ漏らさずに共有します。
    
==== テストケース作成 & 稼働判定シナリオ作成
    
    開発から共有された内容をもとに評価観点を洗い出しテストケースを作成していきます。
    不明点があれば適宜開発メンバーに確認して作成していきます。認識に相違があると重大な不具合が検知されずにリリースされてしまうことになりかねないので、できるだけ開発メンバーと認識をすり合わせながらテストケースの作成をしていきます。
    新たにリリースした機能をユーザーが利用する場合や、初めてリリースされる機能を使って稼働するユーザーがいる場合でビジネス的なインパクトが大きい場合には、該当のお客様の運用に沿ってシナリオを作成します。お客様の運用に耐えられるかの確認やお客様との期待値調整をするために利用され、主にCXが開発から共有された情報をもとに作成します。
    
==== QA評価 & 検証
    
    作成したテストケースをもとに、評価し修正すべきことがあれば情報を整理して開発に伝達します。
    
==== 戻り対応
    
    QA評価の結果やCXの検証結果をもとに修正すべき内容を修正します。
    
==== リリース
    
    満を持してリリース！
    

=== 課題

各プロセスごとに特に以下のような課題を抱えていました。

==== 設計 & デザイン
    * 設計が属人的となってしまっていた
    * 共有はしているものの他の開発メンバーの解像度が低く、実装に着手しても上手く進められないことがある
    * この状態でこの操作したらどうするんだっけ？といったエッジケースの考慮が漏れがちで、後続のタスクでリカバリーが必要となることがある
==== 実装
    * マージまでにリードタイムが発生してしまう
    * 他メンバーに知識が共有されにくく評価戻りや問い合わせ対応で、開発担当者をアサインせざるをえない状態となりやすい
    * エッジケースなどで前提を合わせられておらず、追加でCXやPdMとコミュニーケーションが発生することがある
    * 結果としてタスクが追加されることがある
==== テストケース作成 & 稼働判定シナリオ作成
    * 考慮漏れや十分に精査できていない部分があり、開発中や評価期間中に問題に気づくことがある
    * QA観点を考慮した結果、実装不備が明確となり追加修正が発生することがある
    * 開発からQAに十分に伝達しきれていないことがあり、テスト実施中に認識の齟齬が発覚することがある
    * ここで実施したコミュニケーションで認識の相違があり、後の工程で発覚して問題となることがある
    

=== 課題まとめ

 * 漏れ・不備が後段のプロセスで発覚する
 * 設計や実装が属人的でありスケールしづらい
 * コミュニケーションコストが高い
 * コミュニケーションロスが発生する
 * アセットが少なく、挙動に不安があったり不具合と思しき事象が発生したり詳細な仕様を確認をしたい場合に、都度開発とコミュニケーションを取ったり、開発は実際に動かしたりソースを確認せざるを得なかったりコストが高くなってしまうことがある

== After

=== 開発プロセス

改善後の開発プロセスです。

//image[2024-07-15_1_10_54][改善後の開発プロセス][scale=1.0]{

//}

=== 変更点

==== イベントストーミング
    
    イベントストーミングという手法を用いて、システムとしてどのような振る舞いをするのか何を実現すべきなのか…を開発者だけでなくCX,QA,UI/UXのメンバーがわかる言葉で整理します。
    開発に関わる全メンバーが参加するので、レビューや共有といったプロセスは不要となります。また徹底的に議論するため、全員の解像度の向上と合意を得ることができます。
    エッジケース含めて洗い出しをし、QA目線でも漏れがないかなどを精査しながら議論と整理するため時間はかかりますが、参加者全員でかなり高い精度で認識を合わせることができます。
    
    ※ 詳細は後述参照
    
==== QRDの作成 ( ≒ テストケース作成 )
    
    Quality Requirements Document。イベントストーミングで整理した振る舞いを元に、テストケースの観点ごとにパターンと結果を整理します。主にQAが作成し、PdM, 開発, CX でレビューします。
    Given-When-Then 形式で実現すべき振る舞いをドキュメントにまとめていきます。イベントストーミングで作成した一連のフローの中から、機能単位で特定の領域を抽出してパターンを付与して振る舞いの期待値を記述していきます。
    イベントストーミングで議論されなかった新たな観点やケースが発覚した際には、必要に応じてPdM・開発・CX と相談して決めます。
    
    ※ 詳細は後述参照
    
==== QRSの作成
    
    Quality Requirements Scenario。QRDが機能単位のドキュメントであるのに対して、QRSはシナリオベースとなります。機能単位では上手く動作していたとしても、複数の機能を利用するシナリオではうまく動作しない…といったことも発生し得ます。組み合わせは無数に存在するので…特にプロダクトとして保証すべきシナリオを明文化して、必ず指定のシナリオが保証される状態とします。
    どのシナリオを保証すべきかは、PdM・CXと協議して決めます。
    主にQAが作成を行い、PdM・CXのレビューを経て完了となります。
    
==== 稼働判定シナリオの作成
    
    実施すべきことは概ね同じですが、既に作成済みのQRSをもとに作成するという点で大きく異なります。原則的にはQRSから不要な部分を削減するのみで大幅な追記等はせずに作成します。そうすることでQAの検証範囲でカバーできます。
    
==== 設計 & デザイン
    
    実施すべきことは概ね同じですが、イベントストーミングを全員で実施しQRDも作成済みであるため、振る舞いに関して詳細化されています。そのため、どのように実現するかにより注力できるようになります。もともとはTLに属人化していましたが、詳細化すべきポイントを限定的にすること、必要に応じてペアやモブで実施することで、多くの範囲を開発の全メンバーで実施することができるようになりました。
    デザインについても Before と同様の理由で、同様の理由でコミュニケーションを円滑に回すことができるようになります。
    
==== 実装
    
    Before ではタスクを分解して分担して実施していましたが、Afterでは全員でモブプロもしくはペアプロを取り入れつつTDDに準拠して実装を進めています。。
    モブプロやペアプロを実施しようとすると、どこから着手するかや設計や見通しの解像度に個人差がありスムーズに運用できないこともあります。(経験談)
    イベントストーミングを通じて「全体の解像度を底上げ」「QRDで機能単位ごとに実現すべき振る舞いの明文化」ができたので、QRDの項目を機能優先度が高い順に「テスト」→「実装」→「リファクタ」のサイクルを回してて完了させることができます。
    最初から複数人で実装しているため、レビューを経ることなくマージして完了となります。
    
==== 検証
    
    作成したQRD, QRS, 稼働判定シナリオに基づいて検証します。
    Before では稼働判定シナリオは CX が独自に事前検証していましたが、Afterでは QRSに含まれるので QA の検証で保証されます。
    CXはよりお客様体験に特化した、画面のデザイン含むシステムのインタラクションが与えるお客様への感情的な影響にフォーカスして検証を進めます。
    
==== 戻り対応
    
    QA, CX の検証にて発覚した修正すべき内容に対応します。
    Beforeでは〜ましたが、Afterではこのムラを解消し、誰でも修正可能になりました
    Beforeでは知識にムラがあり実装した人が修正を強いられることが多くありましたが、Afterではこのムラを解消し、誰でも修正可能になりました
    

=== 成果

==== 期待通りの成果

 * 評価戻りが減った！
 * 評価戻りの対応コストが減った！(体感)
 * 仕様が分からなくなったり、疑問を持ったりした時の拠り所ができた！
 * チームを横断してワイワイできたことで、一体感が強まった

==== 期待以上の成果

 * より楽しく開発できるようになった！
 * チーム横断のコミュニケーションが円滑になった&コミュニケーションコストが下がった
 * モブプロ x ペアプロを駆使することで、全員がドメインや実装が良く分かる状態を実現でき、評価戻りや問い合わせが誰でも対応できるようになった！

==== 想定外の成果

 * QAから機能追加の提案が多く出るようになった！
 * 全員が詳しい状態となることで、設計時の観点が増え、より戻りが少ない状態を実現できた！
 * TDDを実施することで、開発プロセスの型化が進んで、注力すべきポイントを限定することができた！
 * モブプロ x TDDの組み合わせが良く、モブプロの進行をスムーズにすることができた！
 * イベントストーミング x BDD (TDD) の相性が良く、開発プロセスが簡易化された！

== 実施したこと

=== イベントストーミング

複雑なビジネスプロセスやドメインの知識を共有、可視化、理解を深めること協同作業でモデリングすることで達成する手法です。ソフトウェア開発者だけでなく、ビジネスステークホルダーやQA / UIUX / データアナリストが同じ手法で議論や理解を進めることが可能です。ビジネスプロセスの”仕組み”とプロダクトの”仕組み”を構造化することで、プロダクトの設計に直結しやすくかつ汎用性の高いアセットを作成することができます。
After のプロセスでは設計やデザインなどを開始する前に、開発・PdM・CX・QA・UI/UXの全員で議論しました。本来ドメインイベントの抽出から全員で実施すべきですが、今回はイベントストーミングの習熟度が高くない、スコープが広く期日が比較的近い機能開発であったため、たたきとなるベースを1人が作成したうえで、それをベースにして全員で議論し詳細化していくという進め方で実行しました。

イベントストーミングの成果物の一例@<fn>{example}

//footnote[example][画像が見えづらいかと思うので、詳細はZennの記事ご覧ください。https://zenn.dev/bitkey_dev/articles/0633337ee7516c]

//image[qrcode_zenn_dev][図が見えづらい場合は、こちらの記事をご覧ください][scale=0.3]{

//}

//image[2024-07-16_17_07_46][成果物の一例1][scale=1.0]{

//}

//image[2024-07-16_17_08_16][成果物の一例2][scale=1.0]{

//}

//image[2024-07-16_17_08_35][成果物の一例3][scale=1.0]{

//}



一部抜粋

//image[part][とあるお客様の一例の抜粋][scale=1.0]{

//}
※ イベントソーシングと合わせて語られることも多いですが、今回イベントソーシングは活用していません。

=== BDD

Behavior Driven Development。ビヘイビア駆動開発や振る舞い駆動開発とも呼ばれ、テスト駆動開発から派生したものです。ソフトウェアの振る舞いや機能を重視して、品質と要件の向上をめざす手法です。
開発が進んだ後にプロダクトの挙動が想定と異なることが発覚したり、エッジケースの考慮が漏れていることが後半のプロセスで発覚することを防ぐことを期待して取り入れました。
After のプロセスにおける、QRD, QRS, 実装 が関連プロセスとなります。
イベントストーミングで実現すべき振る舞いについてかなり詳細に整理し認識合わせ済みではあるものの、ここからさらにQA視点でデータパターンごとの振る舞いをドキュメント(QRD)として整理し関係者で全員で認識合わせます。QRDは Given-When-Then 形式で記述し、開発以外でもわかりやすい記載とします。
その後に実装する際には、QRDで作成した記載に基づき、テスト記述 → 実装 → リファクタ のサイクルを繰り返して実行します。
テスト含めて実装したものについてはQRD上のステータスを更新して、実装が完了したことが誰が見ても明らかな状態とするようにしています。

QRD成果物例

//image[2024-07-16_17_34_05][QRD成果物例][scale=1.0]{

//}

=== TDD

Test-Driven Development。テスト駆動開発。BDDと重複あり。QRDで作成したケースをもとに、以下のサイクルを繰り返すことで実装を進めました。

 1. テスト記述
 2. 実装
 3. リファクタ

今までは何を実装して、何を実装していないか…は開発者…正確にはコードのみぞ知る…みたい状態がちらほらあって、主要な機能は想定通り動作するが、エッジケースのときの挙動はわからない…や期待と異なる…といった問題が度々発生していました。
TDDとすることで、実現したこと = 自動テストを記述して成功しているもの 、として明示できる状態となったので、なにが実装されていて、なにが実装されていないのか、またどのような挙動となるのか、について QAやCX, PdM とのコミュニケーションがしやすくなりました。またQRDと組み合わせることで、QRDに実装ステータスが記載されているので、もはやコミュニケーションを個別にとる必要なく、なにが実装されていてなにが実装されているのか、またどのような挙動となるのか　が簡単に分かる状態とすることができました。

=== モブプロ

モブプロ（モブプログラミング）は、複数のプログラマーが一つのコンピュータを共有して、同時に一つのタスクを解決するソフトウェア開発手法です。この方法は、チーム全員が集まって一つの画面を見ながら、一人がキーボードを操作してコードを書き、他のメンバーがアイデアを出したり、指摘したりすることで協力して作業を進めます。「知識の共有」「品質の向上」「学習効果」「チームワークの強化」などのメリットの享受が期待できます。
チームメンバーは全員で4人なのですが、原則チームメンバー4人全員で実施しました。
規模が小さいものや全員の習熟度が比較的高い領域などは2人x2組のペアを作り、ペアプロで実施しました。
モブプロ単体で導入した当初は、なかなか進行が難しくどこから手を付けてよいかわからない…や、そもそも全員の知識に偏りがあり情報の共有や実装方針の議論で時間を使ってしまうことが頻発していました。
QRD x TDD と組み合わせることで、プロセスを以下ステップに型化することができ、ステップごとのやるべきことが明確となりよりスムーズに進められるようになりました。

 1. QRDから対象となるケースを抽出する
 2. QRDのケースを優先度順に並べる
 3. QRDの優先度が一番高いケースのテストを記述する
 4. テストがクリアするように実装する
 5. リファクタする
 6. QRDのケースの実装ステータスを完了とする
 7. 3〜6を繰り返す

=== DDD

Domain Driven Design (ドメイン駆動開発)。
大きく新しいことを取り入れたわけではないですが…イベントストーミングでビジネスサイドと内容を認識合わせをし、イベントストーミングで抽出した集約の粒度でEntityやRepositoryを実装して進めました。
集約の粒度としてなにが良いか…というのは今までも議論の対象で、なかなか良い評価軸を見つけられていませんでしたが、イベントストーミングを経ることで全員が認知しやすい粒度を追求しやすくなったと感じます。イベントストーミングで取り上げる際に扱いやすい粒度 ≒ 認知しやすい粒度 として積極的にイベントストーミングで整理された粒度を集約の粒度として活用するようにしました。

== 工夫点・成功の要因

=== CX, QA, UI/UX, PdM, 開発メンバー全員が協力的

なんといっても、携わるメンバーの協力が必要です。
事前に現状の課題と新たな取り組みによってどんな状態にすることができるかを説明しますが、積極的に参加してくれるかは全員の協力あってこそです。
この点において特にメンバーには恵まれていると感じます。

=== イベントストーミングの事前準備と振り返り

私含めてイベントストーミングについてはゼロベースでのスタートでした。
イベントストーミングの概要の共有会やYoutubeで公開されているやり方をみんなで視聴したり、実際に既にリリース済みの機能で試しに実施してみたりしました。
試しに実施した後は、振り返りだけでなくGoogleフォームでアンケートを取ったり、アンケート結果に基づいて改善を繰り返しました。全員の時間を抑える必要がある都合上実施から時間を開けざるを得なく、またイベントストーミングの有用性として定量的な材料が欲しかったためです。
もともとは1〜2時間 x 数セットのように分割して実行していましたが、開始時点で前回の実施内容の思い出しが必要で改善したいという声が多かったので、1日まるまるイベントストーミングする時間を2回とって実施しました。また最初はドメインイベントの粒度の認識などを、みんなで1から組み立てつつ揃えることが難しく大枠を１人がざっくりまとめたうえで、それをたたきとして全員で議論するプロセスにするなどしました。

=== BDD, TDD の円滑化

BDDやTDDについてもほぼゼロベースからのスタートでした。そのような中でも円滑に進められるように、QRDのフォーマットを自動テストで取り扱いやすいものにチューニングしたり、自動テストの実装が捗るようにテストを記載しやすい基盤づくりやルール整備などを実施しました。
結果として、QRDの記載事項をそのまま自動テストに展開できるようになり、テストの実装が非常に円滑に進めることができました。

=== 評価もどりの分析

評価戻りのパターンを以下のパターン別に記録して、後で分析できるようにしました。

 1. 対象外 : 自動テストの対象外としているもの。負荷検証など。
 2. ケース漏れ：ケースとして漏れていたもの。QRDの記載から漏れていた、QRDには記載されていたが開発側で漏れていた、不要と判断していた…など。
 3. チェック項目漏れ：ケースとしては含まれていたが、テストでチェックする項目が不足・不備があったもの。
 4. Skipped：他テストとの兼ね合いを考慮して不要と判断したものや、優先度の兼ね合い上対応を見送ったもの。
 5. その他不備：上記以外の不備。ケース、チェック項目は適切であったがテストの実装や準備するデータに不備があった…など。
 6. 要議論：どれに当てはまるか判断できないので、チームで議論して振り返るもの。

分析した結果を次のスプリントで活かして同様の問題が発生しないように少しづつ改善を繰り返しました。TDDを踏襲することで、戻りに対してどのプロセスに改善の余地があるかをより明瞭にすることができるようになりました。

== 課題

=== 想定通りの課題

 * アセットのメンテナンス
    
    イベントストーミングやQRDといった成果物を作成した後、開発など他のプロセスを進めていくうちに更新が必要となることが発生します。この際のプロセスを事前に定められておらず、反映できない内容が発生して、別途時間をとって反映する…といった作業が必要となってしまいました。
    

 * 一時的にアウトプットが減った
    
    当たり前なのですが今までやったことのない取組を実施したので、当初はベロシティが下がりましたが、1-2ヶ月ほどで元の水準に回復することができました。
    さらに、もともとはスプリント開始時に定めたスプリントゴールを達成することができないことも多々ありましたが、全員で実施した結果として、解像度の向上や見積もり精度の向上もあり連続でスプリントゴールを達成することができるようになりました。
    
 * 事前準備が大変だった
    
    イベントストーミングやBDDを関係者全員で実施するための説明や、実施時の準備などは時間がかかりました。特にイベントストーミングは説明だけでなく、実際に手を動かして実施するワークショップ的なものも実施~~したり~~したのでそれらの企画や準備も必要でした。
    
 * 習熟に時間がかかる
    
    イベントストーミングやTDDの習熟度向上には時間がかかりました。
    TDDはテストの実装方法もそうですが、テストしやすいコードとするためにどうする？とか、どこまでテストで確認するべき？などの認識を合わせながら進める必要がありました。
    イベントストーミングは、イベントストーミングのフォーマットの説明だけでなく、どのような粒度でイベントを記述していくかのの認識を合わせるのが非常に大変でした。
    

=== 想定外の課題

 * 段取りが不十分で混乱させてしまうことがあった
    
    最初から After のプロセスが実施できていたわけではなく、最初は試行錯誤しながら進めていたこともあり、QRDが作成されていない状態で、 設計 → 実装 に着手しなければならない…などの状況が発生してしまい、上手く運用できていないタイミングもありました。
    
 * Draw.ioでのイベントストーミングがつらすぎた
    
    イベントストーミングを実施するツールとして、@<href>{http://draw.io, Draw.io}@<fn>{drawio}を使っていました。github上で管理しやすいツールとして Draw.io を選定したのですが、同時編集作業が辛かったです。これは無視できないタイムラグによって、同じ箇所を修正すると、もう片方の修正がもう片方の修正を上書きしてしまう」という問題が多発したからです。
    
//footnote[drawio][http://draw.io]

=== 想定より問題とならなかったこと

 * 一時的にアウトプットが減った
    
    実際にアウトプットが一時的に減ったのですが、目に見えて減ったのはイベントストーミングを集中的に実施した1スプリントくらいで、他のスプリントは大きく低下することなく実施することができました。
    

== 学びと今後の展望

良かった！

比較的規模の大きい案件であっても非常にスムーズに開発を進めることができた。
何よりメンバーが自律的に活動しやすくなり、その結果もあって体感としてより楽しく開発できるようになったことが何よりの成果。
まだすべての開発で改善後のプロセスを踏襲できているわけではなく、現在は規模が大きいプロジェクトでのみの実績しかありません。今後はできるだけすべての開発のプロセスを改善したいのと他チームへの展開も積極的に実施していきたいと考えています。