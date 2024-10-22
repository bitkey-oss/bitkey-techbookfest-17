# ビットキー技術書部 技術書典１７向け原稿リポジトリ


## 使い方
### Docker環境を整える
1. Dockerをインストールする



### PDFを作成する
 以下のコマンドを実行する

```sh
docker run --rm -v `pwd`:/work vvakame/review:5.0 /bin/sh -c "cd /work/articles ; review-pdfmaker config.yml"
```

articlesディレクトリにPDFが作成されている


## 権利



[TechBooster/ReVIEW\-Template](https://github.com/TechBooster/ReVIEW-Template) をカスタマイズして使用しています。

 * 設定ファイル、テンプレートなど制作環境（techbooster-doujin-base.styなど）はMITライセンスです
   * 再配布などMITライセンスで定める範囲で権利者表記をおねがいします
 * articles/styにあるファイルには以下のライセンスが適用されています。
   * review-jsbook.cls, review-base.sty, review-style.sty, review-custom.sty: MIT License
   * jumoline.sty: The LaTeX Project Public License
   * plistings.sty: MIT License
   * gentombow.sty: BSD License
   * jsbook.cls: BSD License